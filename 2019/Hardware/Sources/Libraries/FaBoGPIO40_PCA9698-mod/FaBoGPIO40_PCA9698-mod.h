/**
 @file FaBoGPIO40_PCA9698.h
 @brief This is a library for the FaBo GPIO I2C Brick.

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#ifndef FABOGPIO40_PCA9698_H
#define FABOGPIO40_PCA9698_H

#include <Arduino.h>
#include <Wire.h>

#define PCA9698_SLAVE_ADDRESS 0x20 ///< Default I2C Slave Address

/// @name Register Addresses
/// @{
//#define PCA9698_OUTPUT_REG 0x01
//#define PCA9698_CONFIGURATION_REG 0x03

#define PCA9698_INPUT_PORT0 0x00
#define PCA9698_INPUT_PORT1 0x01
#define PCA9698_OUTPUT_PORT0 0x02
#define PCA9698_OUTPUT_PORT1 0x03
#define PCA9698_CONFIG_PORT0 0x06
#define PCA9698_CONFIG_PORT1 0x07



/// @}

/// @name OUTPUT Parameter
/// @{
#define PORT1
/// @}

/**
 @class FaBoGPIO
 @brief FaBo GPIO I2C Controll class
*/
class FaBoGPIO40 {
  public:
    FaBoGPIO40(uint8_t addr = PCA9698_SLAVE_ADDRESS);
    void configuration(void);
    void digitalWrite(uint8_t pin, uint8_t output);
    void pinWrite(uint8_t pin, uint8_t value);
    void portWrite(uint8_t port, uint8_t value);
	int digitalRead(uint8_t pin);
    void setAllClear();
    void portMode(uint8_t port, uint8_t mode);
  private:
    uint8_t _i2caddr;
    uint8_t _output_port[2];
    void writeI2c(uint8_t address, uint8_t data);
    void readI2c(uint8_t address, uint8_t num, uint8_t * data);
};

#endif // FABOGPIO40_PCA9698_H
