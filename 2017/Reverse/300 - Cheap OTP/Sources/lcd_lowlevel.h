#ifndef LL_H
#define LL_H

/**
 * @file lcd_lowlevel.h
 * @brief LCD Low level commands
 * @details These are the low level LCD commands that user shouldn't never need to
 * call. You shouldn't need to include this header file in your project.
 */

/** Set Display Data RAM (DDRAM) address                */
#define DDRAM                   0x80

/** LCD initialisation status flag */
extern int LL_init;

/**
 * @brief Send a nibble to one of the LCD registers
 * @param[in] data please put the data in the lower nibble.
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 * @return 0, on success
 * @warning: This function DOES NOT change the GPIO direction, so it should
 * not be called on its own.
 */
int LL_write_nibble(uint8_t data, int RS);

/**
 * @brief read a nibble from one of the LCD registers
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 * @return the data will be in the lower nibble.
 * @note please refer to the timing diagram at pg. 22
 */
uint8_t LL_read_nibble(int RS);

/**
 * @brief Send a byte to the LCD screen
 * @details This function calls LL_write_nibble twice.
 * @return
 * - on success: 0
 */
int LL_write_byte(uint8_t data, int RS);

/**
 * @brief read a byte from one of the LCD registers
 * @param[in] RS
 * - 0: Select instruction register
 * - 1: Select data register
 */
uint8_t LL_read_byte(int RS);

/**
 * @brief Poll LCD until it is no longer busy
 */
void LL_busy_wait();

/**
 * @brief Read address counter
 */
uint8_t LL_addr();

#endif
