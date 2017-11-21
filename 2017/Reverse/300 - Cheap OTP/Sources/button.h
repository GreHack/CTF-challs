#ifndef BUTTON_H
#define BUTTON_H
/**
 * @file button.h
 * @brief functions related to the GPIO buttons
 */

#include <stdint.h>

/**
 * @brief The buttons on the panel
 */
typedef enum {
    Select = 1,
    Right = 2,
    Down = 4,
    Up = 8,
    Left = 16,
    Null
} Button;

/**
 * @brief Read button pressing (non-blocking version)
 */
Button btn_nblk();

/**
 * @brief Read raw button data (non-blocking version)
 */
uint8_t btn_nblk_raw();

/**
 * @brief Read button pressing (blocking version)
 */
Button btn_blk();

/**
 * @brief Read raw button data (blocking version)
 * @details result is returned as soon as the button has been pressed down. Use
 * btn_return_clk if you want the wait until the button has been released.
 */
uint8_t btn_blk_raw();

/**
 * @brief return a button click
 * @details A button click is a combination of pressing the button down
 * and releasing the button back up.
 */
Button btn_return_clk();

/**
 * @brief Print a button press to the stdout
 * @return the return from the printf() call
 */
int btn_printf(Button button);

#endif
