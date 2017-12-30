/*
 * The source code is taken from arduino forum:
 * https://forum.arduino.cc/index.php?topic=120049.0
*/

#include <Arduino.h>
#include "Spi.h"

//---------- constructor ----------------------------------------------------
SPI::SPI(){
}

//------------------ mode ---------------------------------------------------
void SPI::mode(byte config){
//  initialize the SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);

//  enable SPI master with configuration byte specified
  SPCR = 0;
  SPCR = (config & 0x7F) | (1<<SPE) | (1<<MSTR);
}

void SPI::SPI_SlaveInit(void){
//  Set MISO output, all others input 
  pinMode(SCK_PIN, INPUT);
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SS_PIN, INPUT);

//  SPIE=0, SPE=1, DORD=0, MSTR=0, CPOL=0, CPOL=0, SPR=00
  SPCR = B01000000;
}

//------------------ receive -----------------------------------------------
uint8_t SPI::SPI_SlaveReceive(void){
  cmd_counter ++;
  SPDR = cmd_counter;
  /* Wait for reception complete */
  while((!(SPSR & (1<<SPIF))) && (digitalRead(SS_PIN) == LOW)){}
  //  while(!(SPSR & (1<<SPIF)))
  /* Return Data Register */
  return SPDR;
}

//------------------ transfer -----------------------------------------------
uint8_t SPI::transfer(uint8_t value){
  SPDR = value;
  while (!(SPSR & (1<<SPIF))) ;
  return SPDR;
}

uint8_t SPI::transfer(uint8_t value, uint8_t period){
  SPDR = value;
  if (period > 0) delayMicroseconds(period);
  while (!(SPSR & (1<<SPIF))) ;
  return SPDR;
}

//---------- preinstantiate SPI object --------------------------------------
SPI Spi = SPI();

