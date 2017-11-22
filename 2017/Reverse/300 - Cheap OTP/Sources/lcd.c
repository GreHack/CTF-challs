#include <stdarg.h>
#include <stdio.h>

#include "gpio.h"
#include "lcd_lowlevel.h"

#include "lcd.h"

/* Opcode definitions - pg.24 of the datasheet          */

/** Special initialisation code for 4-bit mode          */
#define FOUR_BIT_MODE           0x02

/** Clear display                                       */
#define CLEAR                   0x01

/** Return home                                         */
#define HOME                    0x02

/**
 * @brief Function set
 * @details
 * - Interface length = 4 bits,
 * - Display lines = 2,
 * - Character font = 5 x 8 dots
 */
#define FUNCTION_SET            0x28

/** Cursor or display shift                             */
#define SHIFT                   0x10
#define DISPLAY                 0x08
#define CURSOR                  0x00
#define RIGHT                   0x04
#define LEFT                    0x00

/* Other bits and pieces                                */

/** The number of characters DDRAM can store            */
#define DDRAM_LENGTH            80

/** The start of the first row in DDRAM         */
#define DDRAM_1ST_ROW_START     0x00

/** The last address in the first row of DDRAM. */
#define DDRAM_1ST_ROW_END       0x27

/** The start of the second row in DDRAM        */
#define DDRAM_2ND_ROW_START     0x40

int LCD_DISPLAY_SHIFT = 0;

int LCD_init(uint8_t display_control)
{
    if (LL_init == 1) {
        fprintf(stderr, "LCD_init: LCD is already initialised.\n");
        return 0;
    }
    LL_init = 1;
    int r = LCD_cmd(FOUR_BIT_MODE);
    r += LCD_cmd(FUNCTION_SET);
    r += LCD_cmd(DISPLAY_SET | DISPLAY_ON | display_control);
    r += LCD_cmd(ENTRY_MODE_SET | INCREMENT);
    r += LCD_clear();
    if (r == 0) {
        return r;
    }
    LL_init = 0;
    fprintf(stderr, "LCD_init: LCD initialisation failed\n");
    return r;
}

int LCD_cmd(uint8_t cmd)
{
    return LL_write_byte(cmd, 0);
}

int LCD_putchar(char c)
{
    uint8_t addr;
    switch (c) {
    case '\r':
        if (LCD_cursor_addr() < DDRAM_1ST_ROW_END) {
            addr = DDRAM_1ST_ROW_START;
        } else {
            addr = DDRAM_2ND_ROW_START;
        }
        return LCD_cmd(DDRAM | addr);
    case '\n':
        if (LCD_cursor_addr() > DDRAM_1ST_ROW_END) {
            addr = DDRAM_1ST_ROW_START;
        } else {
            addr = DDRAM_2ND_ROW_START;
        }
        return LCD_cmd(DDRAM | addr);
    }
    if (LL_write_byte(c, 1) != 0) {
        return EOF;
    }
    return (unsigned char) c;
}

char LCD_getchar()
{
    char c = LL_read_byte(1);
    LL_busy_wait();
    return c;
}

/**
 * @note This function in fact returns the content of the LCD address counter,
 * which could actually point to CGRAM. However all CGRAM related functions
 * restore the content of the address counter at the end so it points to
 * DDRAM.
 */
uint8_t LCD_cursor_addr()
{
    return LL_addr();
}

int LCD_clear()
{
    int r = LCD_cmd(CLEAR);
    LL_busy_wait();
    LCD_DISPLAY_SHIFT = 0;
    return r;
}

int LCD_home()
{
    int r = LCD_cmd(HOME);
    LCD_DISPLAY_SHIFT = 0;
    return r;
}

int LCD_line_clear()
{
    int r = LCD_putchar('\r');
    for (int i = 0; i < LCD_LENGTH; i++) {
        r += LCD_putchar(' ');
    }
    r += LCD_putchar('\r');
    return r;
}

int LCD_printf(const char *format, ...)
{
    int i = 0;
    va_list arg;
    char s[DDRAM_LENGTH + 1];

    va_start(arg, format);
    vsnprintf(s, DDRAM_LENGTH + 1, format, arg);
    va_end(arg);

    for (i = 0; s[i] != '\0'; i++) {
        LCD_putchar(s[i]);
    }

    return i;
}

int LCD_wrap_printf(const char *format, ...)
{
    LCD_clear();

    int i;
    va_list arg;
    char s[2 * LCD_LENGTH + 1];

    va_start(arg, format);
    vsnprintf(s, 2 * LCD_LENGTH + 1, format, arg);
    va_end(arg);

    for (i = 0; s[i] != '\0'; i++) {
        if (i == LCD_LENGTH) {
            LCD_putchar('\n');
        }
        LCD_putchar(s[i]);
    }

    return i;
}

int LCD_cursor_move(int n)
{
    int i;
    if (n > 0) {
        for (i = 0; i < n; i++) {
            LCD_cmd(SHIFT | CURSOR | RIGHT);
        }
    } else {
        for (i = 0; i > n; i--) {
            LCD_cmd(SHIFT | CURSOR | LEFT);
        }
    }
    return LCD_cursor_addr();
}

/**
 * @brief move the LCD cursor to a certain location on LCD.
 * @param[in] line the new line number
 * @param[in] n the new character number
 * @return the new cursor address
 */
int LCD_cursor_goto(int line, int n)
{
    int r = 0;
    uint8_t addr = 0;
    if (n > DDRAM_LENGTH / 2 - 1) {
        fprintf(stderr,
                "LCD_cursor_goto error: invalid character number detected.\n");
        return -1;
    }
    if (line > 2) {
        fprintf(stderr,
                "LCD_cursor_goto error: invalid line number detected.\n");
        return -1;
    }
    if (line == 1) {
        addr = DDRAM_2ND_ROW_START;
    }

    if (n > LCD_LENGTH - 1) {
        r = 1;
    }
    addr += n;

    LCD_cmd(DDRAM | addr);

    if (LCD_cursor_addr() == addr) {
        return r;
    }
    fprintf(stderr,
            "LCD_cursor_goto error: the resulting address is inconsistent.\n");
    return -1;
}

int LCD_display_shift(int n)
{
    int i = 0;
    if (n > 0) {
        for (i = 0; i < n; i++) {
            LCD_cmd(SHIFT | DISPLAY | RIGHT);
            LCD_DISPLAY_SHIFT++;
        }
    } else {
        for (i = 0; i > n; i--) {
            LCD_cmd(SHIFT | DISPLAY | LEFT);
            LCD_DISPLAY_SHIFT--;
        }
    }
    LCD_DISPLAY_SHIFT %= 40;
    return LCD_DISPLAY_SHIFT;
}

int LCD_colour(Colour colour)
{
    /* Note that logic value '1' actually turns the LEDs off,
     * and logic value '0' turns them on.               */
    switch (colour) {
    case Black:
        GPIOA_buf.pin.RED = 1;
        GPIOA_buf.pin.GREEN = 1;
        GPIOB_buf.pin.BLUE = 1;
        break;
    case Red:
        GPIOA_buf.pin.RED = 0;
        GPIOA_buf.pin.GREEN = 1;
        GPIOB_buf.pin.BLUE = 1;
        break;
    case Yellow:
        GPIOA_buf.pin.RED = 0;
        GPIOA_buf.pin.GREEN = 0;
        GPIOB_buf.pin.BLUE = 1;
        break;
    case Green:
        GPIOA_buf.pin.RED = 1;
        GPIOA_buf.pin.GREEN = 0;
        GPIOB_buf.pin.BLUE = 1;
        break;
    case Cyan:
        GPIOA_buf.pin.RED = 1;
        GPIOA_buf.pin.GREEN = 0;
        GPIOB_buf.pin.BLUE = 0;
        break;
    case Blue:
        GPIOA_buf.pin.RED = 1;
        GPIOA_buf.pin.GREEN = 1;
        GPIOB_buf.pin.BLUE = 0;
        break;
    case Magenta:
        GPIOA_buf.pin.RED = 0;
        GPIOA_buf.pin.GREEN = 1;
        GPIOB_buf.pin.BLUE = 0;
        break;
    case White:
        GPIOA_buf.pin.RED = 0;
        GPIOA_buf.pin.GREEN = 0;
        GPIOB_buf.pin.BLUE = 0;
        break;
    }
    int r;
    r = GPIO_write(PortA);
    r += GPIO_write(PortB);

    if (r == 0) {
        return r;
    }
    fprintf(stderr, "LCD_colour: Colour change error: %d\n", r);
    return r;
}

int LCD_off()
{
    int r = LCD_cmd(DISPLAY_SET);
    r += LCD_colour(Black);
    r += GPIO_close();
    return r;
}
