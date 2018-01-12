/*!
  LTC6802-1 Multicell Battery Monitor

@verbatim
  The LTC6802 is a 2nd generation multicell battery stack monitor that 
  measures up to 12 series connected cells. The cell measurement range 
  of -0.3V to 5V makes the LTC6802 suitable for most battery 
  chemistries.

  Using the LTC6802-1, multiple devices are connected in
  a daisy-chain with one host processor connection for all
  devices.
@endverbatim

http://www.linear.com/product/LTC6802-1

http://www.linear.com/product/LTC6802-1#demoboards

REVISION HISTORY
$Revision:  $
$Date: 2017-11-28 15:09:16 -0800 $

Copyright (c) 2017, Dr.Ing. Mohammed Nour A. Ahmed
All rights reserved.
some parts of the code are adopted from Linear Technology Corp.(LTC) Linduino library:
http://www.linear.com/solutions/Linduino
*/

//! @ingroup BMS
//! @{
//! @defgroup LTC68021 LTC6802-1: Multicell Battery Monitor
//! @}

/*! @file
    @ingroup LTC68021
    Library for LTC6802-1 Multicell Battery Monitor
*/
/****************************************************************************/

#include <stdint.h>
#include <Arduino.h>
#include "hardwareDescription.h"
#include "LT_SPI.h"
#include "LTC68021.h"
#include <SPI.h>

/***************************************************************************
***********6802 Functions***************************************************
***************************************************************************/


//Initializes the SPI port
void LTC6802_initialize()
{
  // quikeval_SPI_connect();
  spi_enable(SPI_CLOCK_DIV16); // This will set the Linduino to have a 1MHz Clock

}

//Function that writes configuration of LTC6802-1/-3
void LTC6802_wrcfg(uint8_t total_ic,uint8_t config[][6])
{
  uint8_t BYTES_IN_REG = 6;
  uint8_t CMD_LEN = 2+(7*total_ic);
  uint8_t *cmd;
  uint16_t cfg_pec;
  uint8_t cmd_index; //command counter

  cmd = (uint8_t *)malloc(CMD_LEN*sizeof(uint8_t));

  cmd[0] = WRCFG;
  cmd[1] = WRCFG_PEC;
  
  cmd_index = 2;
  for (uint8_t current_ic = total_ic; current_ic > 0; current_ic--){
    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++){
      cmd[cmd_index] = config[current_ic-1][current_byte];
      cmd_index ++;
    }

    cfg_pec = pec8_calc(BYTES_IN_REG, &config[current_ic-1][0]);    // calculating the PEC for each ICs configuration register data
    cmd[cmd_index ] = (uint8_t)cfg_pec;
    cmd_index ++;
  }

  //steps given in data sheet pp.27
  output_low(LTC6802_CS);
  spi_write_array(CMD_LEN, cmd);
  output_high(LTC6802_CS);
  serialPrint(CMD_LEN, cmd);
  free(cmd);
}

void serialPrint(uint8_t CMD_LEN,uint8_t *cmd){
  for (uint8_t i=0 ; i<CMD_LEN ; i++){
    Serial.print(cmd[i], HEX);
    Serial.print(",  ");
  }
  Serial.println();
}


//brief Function that reads configuration of LTC6802-1/-3
int8_t LTC6802_rdcfg(uint8_t total_ic, //Number of ICs in the system
                     uint8_t r_config[][7] //A two dimensional array that the function stores the read configuration data.
                    )
{
  uint8_t BYTES_IN_REG = 7;

  uint8_t cmd[1];
  uint8_t *rx_data;
  int8_t pec_error = 0;
  uint8_t data_pec;
  uint8_t received_pec;

  rx_data = (uint8_t *) malloc((BYTES_IN_REG*total_ic)*sizeof(uint8_t));

  //1
  cmd[0] = 0x02; //RDCFG


  output_low(LTC6802_CS);
  spi_write_read(cmd, 1, rx_data, (BYTES_IN_REG*total_ic)); //Read the configuration data of all ICs on the daisy chain into
  output_high(LTC6802_CS);                          		//rx_data[] array

	//executes for each LTC6802 in the daisy chain and packs the data
    //into the r_config array as well as check the received Config data
    //for any bit errors
  for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++){
    //4.a
    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++){
      r_config[current_ic][current_byte] = rx_data[current_byte + (current_ic*BYTES_IN_REG)];
    }
    //4.b
    received_pec =  r_config[current_ic][6];
    data_pec = pec8_calc(6, &r_config[current_ic][0]);
    if (received_pec != data_pec){ pec_error = -1; }
  }

  free(rx_data);
  //5
  return(pec_error);
}


//Function to start Cell Voltage measurement
void LTC6802_stcvad()
{
  output_low(LTC6802_CS);
  spi_write(STCVAD);
  spi_write(STCVAD_PEC);
  output_high(LTC6802_CS);
}


//Function to start Temp channel voltage measurement
void LTC6802_sttmpad()
{
  output_low(LTC6802_CS);
  spi_write(STTMPAD);
  spi_write(STTMPAD_PEC);
  output_high(LTC6802_CS);
}



//Function that reads Temp Voltage registers
int8_t LTC6802_rdtmp(uint8_t total_ic, uint16_t temp_codes[][3]){
  int data_counter = 0;
  int pec_error = 0;
  uint8_t data_pec = 0;
  uint8_t received_pec = 0;
  uint8_t *rx_data;
  rx_data = (uint8_t *) malloc((6*total_ic)*sizeof(uint8_t));

  output_low(LTC6802_CS);
  spi_write(RDTMP);
  spi_write(RDTMP_PEC);
  for (int i=0; i<total_ic; i++){
    for ( int j = 0; j<6 ; j++){
      rx_data[data_counter] =spi_read(0xFF);
      data_counter++;
    }
  }
  output_high(LTC6802_CS);

  int cell_counter = 0;
  data_counter = 0;
  int temp,temp2;

  for (int j =0; j<total_ic; j++)
  {
    received_pec =  rx_data[5 +(6*j)];
    data_pec = pec8_calc(5, &rx_data[(6*j)]);
    if (received_pec != data_pec){
      pec_error = -1;
      data_counter += 6;
    }else{
      temp = rx_data[data_counter];
      data_counter++;
      temp2 = (uint16_t)((rx_data[data_counter]& 0x0F)<<8);
      temp_codes[j][0] = temp + temp2 - 512;
      
      temp2 = (rx_data[data_counter])>>4;
      data_counter++;
      temp =  (rx_data[data_counter])<<4;
      temp_codes[j][1] = temp + temp2 - 512;

      data_counter++;
      temp2 = (rx_data[data_counter]);
      data_counter++;
      temp =  (rx_data[data_counter]& 0x0F)<<8;
      temp_codes[j][2] = temp + temp2 - 512;
      data_counter+=2;
    }
  }
  free(rx_data);
  return(pec_error);
}


// Function that reads Cell Voltage registers
uint8_t LTC6802_rdcv( uint8_t total_ic, uint16_t cell_codes[][12]){
  
  int data_counter =0;
  int pec_error = 0;
  uint8_t data_pec = 0;
  uint8_t received_pec = 0;
  uint8_t *rx_data;
  rx_data = (uint8_t *) malloc((19*total_ic)*sizeof(uint8_t));

  output_low(LTC6802_CS);
  spi_write(RDCV);
  spi_write(RDCV_PEC);
  for (int i=0; i<total_ic; i++){
    for ( int j = 0; j<19 ; j++){
      rx_data[data_counter++] =spi_read(0xFF);
    }
  }
  output_high(LTC6802_CS);

  int cell_counter = 0;
  data_counter = 0;
  uint16_t temp,temp2;

  for (int j =0; j<total_ic; j++){

    received_pec =  rx_data[18 +(19*j)];
    data_pec = pec8_calc(18, &rx_data[(19*j)]);
    
    if (received_pec != data_pec){
      pec_error = -1;
      data_counter+=19;
    }else{
      for (int k = 0; k<12; k+=2){
        temp = rx_data[data_counter];
        data_counter++;
        temp2 = (uint16_t)((rx_data[data_counter]&0x0F)<<8);
        cell_codes[j][k] = temp + temp2 - 512;
        
        temp2 = (rx_data[data_counter])>>4;
        data_counter++;
        temp =  (rx_data[data_counter])<<4;
        cell_codes[j][k+1] = temp + temp2 - 512;
      }
      data_counter+=2;
    }
  }
  free(rx_data);
  return(pec_error);
}



//Function that calculates PEC byte
uint8_t pec8_calc(uint8_t len, uint8_t *data){
  uint8_t  remainder = 0x41;//PEC_SEED;
  
  // Perform modulo-2 division, a byte at a time.
  for (int byte = 0; byte < len; ++byte){
    
    // Bring the next byte into the remainder.
    remainder ^= data[byte];
    
    // Perform modulo-2 division, a bit at a time.
    for (uint8_t bit = 8; bit > 0; --bit){
      
      // Try to divide the current data bit.
      if (remainder & 128){
        remainder = (remainder << 1) ^ PEC_POLY;
      }else{
        remainder = (remainder << 1);
      }
    }
  }

// The final remainder is the CRC result.
  return (remainder);
}


//Writes an array of bytes out of the SPI port
void spi_write_array(uint8_t len, uint8_t data[]){
  for (uint8_t i = 0; i < len; i++){
    spi_write((int8_t)data[i]);
  }
}


//Writes and read a set number of bytes using the SPI port.
void spi_write_read(uint8_t tx_Data[], uint8_t tx_len, uint8_t *rx_data, uint8_t rx_len){
  for (uint8_t i = 0; i < tx_len; i++){
    spi_write(tx_Data[i]);
  }
  for (uint8_t i = 0; i < rx_len; i++){ 
    rx_data[i] = (uint8_t)spi_read(0xFF);
  }
}
