#ifndef EXP_H
#define EXP_H

/**
 * @file gpio.h
 * @brief functions related to the MCP23017 I/O expander
 * @note for GPIO related operations, please use GPIO_read and GPIO_write,
 * as they update the local buffer.
 */

#include <stdint.h>

/**
 * @brief GPIO Port Enumeration
 */
typedef enum {
    PortA = 0x00,       /**<      Expander Port A         */
    PortB = 0x01        /**<      Expander Port B         */
} Port;

/**
 * @brief GPIO A buffer type definition
 * @details LSB is at the top, MSB is at the bottom!
 */
typedef union {
    uint8_t reg;
    struct {
        unsigned int SELECT:1;
        unsigned int RIGHT:1;
        unsigned int DOWN:1;
        unsigned int UP:1;
        unsigned int LEFT:1;
        unsigned int:1;
        unsigned int RED:1;
        unsigned int GREEN:1;
    } pin;
} GPIOA_BUF_t;

/**
 * @brief GPIO B buffer type definition
 * @details LSB is at the top, MSB is at the bottom!
 */
typedef union {
    uint8_t reg;
    struct {
        unsigned int BLUE:1;
        unsigned int DB7:1;
        unsigned int DB6:1;
        unsigned int DB5:1;
        unsigned int DB4:1;
        unsigned int E:1;
        unsigned int R:1;
        unsigned int RS:1;
    } pin;
} GPIOB_BUF_t;

/**
 * @brief GPIO buffer for Port A
 * @note Modify this buffer then call GPIO_write(PortA)
 */
extern GPIOA_BUF_t GPIOA_buf;

/**
 * @brief GPIO buffer for Port B
 * @note Modify this buffer then call GPIO_write(PortB)
 */
extern GPIOB_BUF_t GPIOB_buf;

/**
 * @brief Initialise the I/O Expander
 * @return
 * - on success: 0
 * - on partial failure: the number of bytes sent to the expander
 * - on failure related to open/ioctl: -1
 */
int GPIO_open();

/**
 * @brief close down the I/O Expander
 * @return 0, on success
 */
int GPIO_close();

/**
 * @brief change GPIO port polarity
 * @return 0, on success
 */
int GPIO_direction(Port port, uint8_t polarity);

/**
 * @brief Write the values in the GPIO buffer to the GPIO register
 * @note GPIO buffers are GPIOA_buf and GPIOB_buf.
 * @return 0, on success
 */
int GPIO_write(Port port);

/**
 * @brief Read the values on the GPIO register to GPIO buffer
 */
uint8_t GPIO_read(Port port);

#endif
