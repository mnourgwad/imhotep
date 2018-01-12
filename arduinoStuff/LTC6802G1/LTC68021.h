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


/* commands set and corresponding PEC */
#define WRCFG 0x01      // Write Configuration Register Group
#define WRCFG_PEC 0xC7
#define RDCFG 0x02      // Read Configuration Register Group
#define RDCFG_PEC 0xCE
#define RDCV 0x04       // Read All Cell Voltage Group
#define RDCV_PEC 0xDC
#define RDCVA 0x06      // Read Cell Voltages 1-4
#define RDCVA_PEC 0x02
#define RDCVB 0x08      // Read Cell Voltages 5-8
#define RDCVB_PEC 0xF8
#define RDCVC 0x0A      // Read Cell Voltages 9-12
#define RDCVC_PEC 0xF6
#define RDFLG 0x0C      // Read Flag Register Group
#define RDFLG_PEC 0xE4
#define RDTMP 0x0E      // Read Temperature Register Group
#define RDTMP_PEC 0xEA

// Start Cell Voltage ADC Conversions and Poll Status
#define STCVAD 0x10 
#define STCVAD_PEC 0xB0
#define STCVAD_CELL_1 0x11
#define STCVAD_CELL_1_PEC 0xB7
#define STCVAD_CELL_2 0x12
#define STCVAD_CELL_2_PEC 0xBE
#define STCVAD_CELL_3 0x13
#define STCVAD_CELL_3_PEC 0xB9
#define STCVAD_CELL_4 0x14
#define STCVAD_CELL_4_PEC 0xAC
#define STCVAD_CELL_5 0x15
#define STCVAD_CELL_5_PEC 0xAB
#define STCVAD_CELL_6 0x16
#define STCVAD_CELL_6_PEC 0xA2
#define STCVAD_CELL_7 0x17
#define STCVAD_CELL_7_PEC 0xA5
#define STCVAD_CELL_8 0x18
#define STCVAD_CELL_8_PEC 0x88
#define STCVAD_CELL_9 0x19
#define STCVAD_CELL_9_PEC 0x8F
#define STCVAD_CELL_10 0x1A
#define STCVAD_CELL_10_PEC 0x86
#define STCVAD_CELL_11 0x1B
#define STCVAD_CELL_11_PEC 0x81
#define STCVAD_CELL_12 0x1C
#define STCVAD_CELL_12_PEC 0x94
#define STCVAD_CLEAR 0x1D
#define STCVAD_CLEAR_PEC 0x93
#define STCVAD_SELFTEST1 0x1E
#define STCVAD_SELFTEST1_PEC 0x9A
#define STCVAD_SELFTEST2 0x1F
#define STCVAD_SELFTEST2_PEC 0x9D

// Start Open-Wire ADC Conversions and Poll Status
#define STOWAD 0x20 
#define STOWAD_PEC 0x20
#define STOWAD_CELL_1 0x21
#define STOWAD_CELL_1_PEC 0x27
#define STOWAD_CELL_2 0x22
#define STOWAD_CELL_2_PEC 0x2E
#define STOWAD_CELL_3 0x23
#define STOWAD_CELL_3_PEC 0x29
#define STOWAD_CELL_4 0x24
#define STOWAD_CELL_4_PEC 0x3C
#define STOWAD_CELL_5 0x25
#define STOWAD_CELL_5_PEC 0x3B
#define STOWAD_CELL_6 0x26
#define STOWAD_CELL_6_PEC 0x32
#define STOWAD_CELL_7 0x27
#define STOWAD_CELL_7_PEC 0x35
#define STOWAD_CELL_8 0x28
#define STOWAD_CELL_8_PEC 0x18
#define STOWAD_CELL_9 0x29
#define STOWAD_CELL_9_PEC 0x1F
#define STOWAD_CELL_10 0x2A
#define STOWAD_CELL_10_PEC 0x16
#define STOWAD_CELL_11 0x2B
#define STOWAD_CELL_11_PEC 0x11
#define STOWAD_CELL_12 0x2C
#define STOWAD_CELL_12_PEC 0x04

// Start Temperature ADC Conversions and Poll Status
#define STTMPAD 0x30
#define STTMPAD_PEC 0x50
#define STTMPAD_EXTERNAL1 0x31
#define STTMPAD_EXTERNAL1_PEC 0x57
#define STTMPAD_EXTERNAL1 0x32
#define STTMPAD_EXTERNAL1_PEC 0x5E
#define STTMPAD_INTERNAL 0x33
#define STTMPAD_INERNAL_PEC 0x59
#define STTMPAD_SELFTEST1 0x3E
#define STTMPAD_SELFTEST1_PEC 0x7A
#define STTMPAD_SELFTEST2 0x3F
#define STTMPAD_SELFTEST2_PEC 0x7D

// Poll ADC Converter Status
#define PLADC 0x40
#define PLADC_PEC 0x07

// Poll Interrupt Status
#define PLINT 0x50
#define PLINT_PEC 0x77

// Start Diagnose and Poll Status
#define DAGN 0x52
#define DAGN_PEC 0x79

// Read Diagnostic Register
#define RDDGNR 0x54
#define RDDGNR_PEC 0x6B

// Start Cell Voltage ADC Conversions and Poll Status, with Discharge Permitted
#define STCVDC 0x60
#define STCVDC_PEC 0xE7
#define STCVDC_CELL_1 0x61
#define STCVDC_CELL_1_PEC 0xE0
#define STCVDC_CELL_2 0x62
#define STCVDC_CELL_2_PEC 0xE9
#define STCVDC_CELL_3 0x63
#define STCVDC_CELL_3_PEC 0xEE
#define STCVDC_CELL_4 0x64
#define STCVDC_CELL_4_PEC 0xFB
#define STCVDC_CELL_5 0x65
#define STCVDC_CELL_5_PEC 0xFC
#define STCVDC_CELL_6 0x66
#define STCVDC_CELL_6_PEC 0xF5
#define STCVDC_CELL_7 0x67
#define STCVDC_CELL_7_PEC 0xF2
#define STCVDC_CELL_8 0x68
#define STCVDC_CELL_8_PEC 0xDF
#define STCVDC_CELL_9 0x69
#define STCVDC_CELL_9_PEC 0xD8
#define STCVDC_CELL_10 0x6A
#define STCVDC_CELL_10_PEC 0xD1
#define STCVDC_CELL_11 0x6B
#define STCVDC_CELL_11_PEC 0xD6
#define STCVDC_CELL_12 0x6C
#define STCVDC_CELL_12_PEC 0xC3

// Start Open-Wire ADC Conversions and Poll Status, with Discharge Permitted
#define STOWDC 0x70
#define STOWDC_PEC 0x97
#define STOWDC_CELL_1 0x71
#define STOWDC_CELL_1_PEC 0x90
#define STOWDC_CELL_2 0x72
#define STOWDC_CELL_2_PEC 0x99
#define STOWDC_CELL_3 0x73
#define STOWDC_CELL_3_PEC 0x9E
#define STOWDC_CELL_4 0x74
#define STOWDC_CELL_4_PEC 0x8B
#define STOWDC_CELL_5 0x75
#define STOWDC_CELL_5_PEC 0x8C
#define STOWDC_CELL_6 0x76
#define STOWDC_CELL_6_PEC 0x85
#define STOWDC_CELL_7 0x77
#define STOWDC_CELL_7_PEC 0x82
#define STOWDC_CELL_8 0x78
#define STOWDC_CELL_8_PEC 0xAF
#define STOWDC_CELL_9 0x79
#define STOWDC_CELL_9_PEC 0xA8
#define STOWDC_CELL_10 0x7A
#define STOWDC_CELL_10_PEC 0xA1
#define STOWDC_CELL_11 0x7B
#define STOWDC_CELL_11_PEC 0xA6
#define STOWDC_CELL_12 0x7C
#define STOWDC_CELL_12_PEC 0xB3

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

void serialPrint(uint8_t CMD_LEN,uint8_t *cmd);

#endif
