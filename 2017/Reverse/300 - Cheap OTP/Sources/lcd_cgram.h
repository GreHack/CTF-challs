#ifndef CGRAM_H
#define CGRAM_H
/**
 * @file lcd_cgram.h
 * @brief functions related to the CGRAM of the LCD
 * @details More details on the custom character bit pattern can be found at
 * pg.19 of the datasheet. You can have 8 custom characters at once. They can
 * be recalled by writing 0 to 7 to DDRAM, for custom character 0 to 7
 * respectively.
 */

/**
 * @brief write a custom character to CGRAM
 * @param[in] n the custom character number
 * @param[in] data an uint8_t array with 8 elements containing the custom
 * character bit pattern
 * @return 0 on success
 */
int CGRAM_write_character(int n, uint8_t data[8]);

/**
 * @brief read a custom character from the CGRAM
 * @param[in] n the custom character number
 * @param[in] data an uint8_t array with 8 elements for storing the custom
 * character bit pattern
 * @return 0 on success

 */
int CGRAM_read_character(int n, uint8_t data[8]);

#endif
