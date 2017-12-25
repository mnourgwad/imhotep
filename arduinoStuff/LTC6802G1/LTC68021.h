/*!
LTC6802-1 Multicell Battery Monitor

http://www.linear.com/product/LTC6802-1

http://www.linear.com/product/LTC6802-1#demoboards
*/

/*! @file
    @ingroup LTC68021
    Library Header File for LTC6802-1 Multicell Battery Monitor
*/
/****************************************************************************/


#ifndef LTC68021_H
#define LTC68021_H

//CSBI pin
#ifndef LTC6802_CS
#define LTC6802_CS QUIKEVAL_CS
#endif

#define PEC_POLY 7
//!Initializes the SPI port
//! @return void
void LTC6802_initialize();

//! Function to start Cell Voltage measurement
//! @return void
void LTC6802_stcvad();

//! Function to start Temp channel voltage measurement
//! @return void
void LTC6802_sttmpad();

//! Function that reads Cell Voltage registers
//! @returns  This function will return a 0 if there is no PEC error and will return -1 if there is a PEC error
uint8_t LTC6802_rdcv(uint8_t total_ic,      //!< total_ic number of LTC6803 ICs in stack
                     uint16_t cell_codes[][12]  //!< The Function will put the parsed measured cell voltages into this array
                    );

//! Function that reads Temp Voltage registers
//! @returns  This function will return a 0 if there is no PEC error and will return -1 if there is a PEC error
int8_t LTC6802_rdtmp(uint8_t total_ic,      //!< total_ic number of LTC6803 ICs in stack
                     uint16_t temp_codes[][3] //!< The Function will put the parsed measured Temp voltages into this array
                    );

//! Function that writes configuration of LTC6802-1/-3
//! @return void
void LTC6802_wrcfg(uint8_t total_ic,      //!< total_ic number of LTC6803 ICs in stack
                   uint8_t config[][6]      //!< The function will write the 6803 CFGR register with data in the config array
                  );

//! Function that reads configuration of LTC6802-1/-3
//! @returns  This function will return a 0 if there is no PEC error and will return -1 if there is a PEC error
int8_t LTC6802_rdcfg(uint8_t total_ic,      //!< total_ic number of LTC6803 ICs in stack
                     uint8_t r_config[][7]    //!< The Function will put the read config register data into this array
                    );
//! Function that calculates PEC byte
//! @returns The calculated CRC8
uint8_t pec8_calc(uint8_t len,          //!< the length of the data array
                  uint8_t *data         //!< data array
                 );

//! Writes an array of bytes out of the SPI port
//! @return void
void spi_write_array(uint8_t length,      //!< length of the data array being written on the SPI port
                     uint8_t *data        //!< the data array to be written on the SPI port
                    );

//! Writes and read a set number of bytes using the SPI port.
//! @return void
void spi_write_read(uint8_t *TxData,      //!< array of data to be written on the SPI port
                    uint8_t TXlen,        //!< length of the tx_data array
                    uint8_t *rx_data,     //!< array that read data will be written too.
                    uint8_t RXlen       //!< number of bytes to be read from the SPI port.
                   );

#endif
