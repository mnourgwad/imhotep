/*
 * The source code is taken from arduino forum:
 * https://forum.arduino.cc/index.php?topic=120049.0
*/

#ifndef Spi_h
#define Spi_h

#include <Arduino.h>
#include <inttypes.h>

#define SCK_PIN   13
#define MISO_PIN  12
#define MOSI_PIN  11
#define SS_PIN    10

class SPI{
  public:
    volatile uint8_t cmd_counter = 0;
    SPI(void);
    void mode(uint8_t);
    void SPI_SlaveInit(void);
    uint8_t SPI_SlaveReceive(void);
    uint8_t transfer(uint8_t);
    uint8_t transfer(uint8_t, uint8_t);
};

extern SPI Spi;

#endif
