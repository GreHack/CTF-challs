#include <stdint.h>
#include <stdio.h>

#include "lcd_lowlevel.h"
#include "lcd.h"

/** Set Character Generation RAM (CGRAM) address */
#define CGRAM                   0x40

static uint8_t g_DDRAM_addr;

/**
 * @brief save DDRAM address
 * @return the saved DDRAM address
 */
static uint8_t DDRAM_addr_save()
{
    g_DDRAM_addr = LL_addr();
    return g_DDRAM_addr;
}

/**
 * @brief restore DDRAM address
 * @return 0, if successful; -1, if failed
 */
static int DDRAM_addr_restore()
{
    uint8_t old_DDRAM_addr = g_DDRAM_addr;
    LCD_cmd(DDRAM | g_DDRAM_addr);
    if (old_DDRAM_addr == g_DDRAM_addr) {
        return 0;
    }
    fprintf(stderr,
            "LL_DDRAM_addr_restore error: could not restore DDRAM address");
    return -1;
}

/**
 * @brief set CGRAM address
 * @param[in] n custom character number, must be no bigger than 7.
 * @return 0, if successful
 */
static int CGRAM_set_addr(int n)
{
    if (n > 7) {
        fprintf(stderr, "CGRAM_set_addr error: invalid address.");
        return -1;
    }
    uint8_t cmd = CGRAM | (n << 3);
    return LCD_cmd(cmd);
}

int CGRAM_write_character(int n, uint8_t data[8])
{
    int r = 0;

    DDRAM_addr_save();
    r += CGRAM_set_addr(n);

    for (int i = 0; i < 8; i++) {
        r += LL_write_byte(data[i], 1);
    }
    r += DDRAM_addr_restore();
    if (r != 0) {
        fprintf(stderr, "CGRAM_write_character error.\n");
    }
    return r;
}

/** @note error check is only performed for CGRAM_set_addr and
 * DDRAM_addr_restore
 */
int CGRAM_read_character(int n, uint8_t data[8])
{
    int r = 0;

    DDRAM_addr_save();
    r += CGRAM_set_addr(n);
    for (int i = 0; i < 8; i++) {
        data[i] = LL_read_byte(1);
    }
    LL_busy_wait();
    r += DDRAM_addr_restore();
    if (r != 0) {
        fprintf(stderr, "CGRAM_read_character error.\n");
    }
    return r;
}
