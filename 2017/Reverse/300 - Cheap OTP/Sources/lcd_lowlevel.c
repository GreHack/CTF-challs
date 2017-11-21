/**
 * @file lcd_lowlevel.c
 * @details
 * - The data pins for the LCD are connected in the reverse order on the
 * I/O expander. Hence we need to manually map pins.
 * - the send/receive nibble commands might cause misalignment in command
 * sequences. Please use them with caution.
 */

#include <stdio.h>

#include "gpio.h"

/** GPIO Port B direction for read                       */
#define LCD_READ                0x1E
/** GPIO Port B direction for write                      */
#define LCD_WRITE               0x00
/** Busy Flag -- DB7 pin                                */
#define BUSY_FLAG               0x80

/** LCD initialisation status flag */
int LL_init = 0;

typedef union {
    uint8_t byte;
    struct {
        unsigned int _0:1;
        unsigned int _1:1;
        unsigned int _2:1;
        unsigned int _3:1;
        unsigned int _4:1;
        unsigned int _5:1;
        unsigned int _6:1;
        unsigned int _7:1;
    } bit;
} Byte;

int LL_write_nibble(uint8_t data, int RS)
{
    int r;
    Byte b;

    b.byte = data;
    /* Set control pins */
    GPIOB_buf.pin.RS = RS;
    GPIOB_buf.pin.R = 0;

    /* Set data pins */
    GPIOB_buf.pin.DB7 = b.bit._3;
    GPIOB_buf.pin.DB6 = b.bit._2;
    GPIOB_buf.pin.DB5 = b.bit._1;
    GPIOB_buf.pin.DB4 = b.bit._0;

    /* Flip the E-pin.                                          */
    /* Note that data is clocked in at the lowering edge.       */
    GPIOB_buf.pin.E = 1;
    r = GPIO_write(PortB);
    GPIOB_buf.pin.E = 0;
    r += GPIO_write(PortB);
    return r;
}

uint8_t LL_read_nibble(int RS)
{
    Byte b;
    b.byte = 0;

    /* Set control pins */
    GPIOB_buf.pin.RS = RS;
    GPIOB_buf.pin.R = 1;

    /* Flip the E-pin.                                          */
    /* Note that data is clocked in at the rising edge.         */
    GPIOB_buf.pin.E = 0;
    GPIO_write(PortB);
    GPIOB_buf.pin.E = 1;
    GPIO_write(PortB);

    /* The actual read operation */
    GPIO_read(PortB);

    /* Copy back the data */
    b.bit._3 = GPIOB_buf.pin.DB7;
    b.bit._2 = GPIOB_buf.pin.DB6;
    b.bit._1 = GPIOB_buf.pin.DB5;
    b.bit._0 = GPIOB_buf.pin.DB4;
    return b.byte;
}

int LL_write_byte(uint8_t data, int RS)
{
    if (!LL_init) {
        fprintf(stderr, "LL_write_byte error: LCD is not initialised!\n");
        return -1;
    }
    int r;
    r = GPIO_direction(PortB, LCD_WRITE);
    r += LL_write_nibble(data >> 4, RS);
    r += LL_write_nibble(data, RS);
    return r;
}

uint8_t LL_read_byte(int RS)
{
    if (!LL_init) {
        fprintf(stderr, "LL_read_byte error: LCD is not initialised!\n");
        return -1;
    }
    uint8_t r;
    GPIO_direction(PortB, LCD_READ);
    r = LL_read_nibble(RS) << 4;
    r |= LL_read_nibble(RS);
    return r;
}


void LL_busy_wait()
{
    while (LL_read_byte(0) & BUSY_FLAG);
}

uint8_t LL_addr()
{
    return LL_read_byte(0) & ~BUSY_FLAG;
}
