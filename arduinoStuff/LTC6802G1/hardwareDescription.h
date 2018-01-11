//! @todo Review this file.
/*
hardwareDescription.h

This file contains the hardware definitions for the Multicell Battery Monitor.

REVISION HISTORY
$Revision:  $
$Date: 2017-11-28 15:09:16 -0800 $

Copyright (c) 2017, Dr.Ing. Mohammed Nour A. Ahmed
All rights reserved.
some parts of the code are adopted from Linear Technology Corp.(LTC) Linduino library:
http://www.linear.com/solutions/Linduino
*/

/*! @file
    @ingroup hardwareDescription
    Header File for hardwareDescription Libraries and Demo Code
*/
/****************************************************************************/

#ifndef HARDWAREdESCRIPTION_H
#define HARDWAREdESCRIPTION_H

#include <Arduino.h>  // typedefs use types defined in this header file.

//! @name LINDUINO PIN ASSIGNMENTS
//! @{

#define QUIKEVAL_GPIO 9          //!< Linduino QuikEval GPIO pin (QuikEval connector pin 14) connects to Arduino pin 9
#define QUIKEVAL_CS SS           //!< QuikEval CS pin (SPI chip select on QuikEval connector pin 6) connects to Arduino SS pin.
#define QUIKEVAL_MUX_MODE_PIN 8  /*!< QUIKEVAL_MUX_MODE_PIN defines the control pin for the QuikEval MUX.
The I2C port's SCL and the SPI port's SCK signals share the same pin on the Linduino's QuikEval connector.
Additionally, the I2C port's SDA and the SPI port's MOSI signals share the same pin on the Linduino's QuikEval connector.
The pair of pins connected to the QuikEval connector is switched using a MUX on the Linduino board.
The control pin to switch the MUX is defined as QUIKEVAL_MUX_MODE_PIN (Arduino pin 8). */

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
#define STCVAD_ALL 0x10 
#define STCVAD_ALL_PEC 0xB0
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
#define STOWAD_ALL 0x20 
#define STOWAD_ALL_PEC 0x20
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
#define STTMPAD_ALL 0x30
#define STTMPAD_ALL_PEC 0x50
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
#define PLINT_PEC 0x79

// Read Diagnostic Register


// Start Cell Voltage ADC Conversions and Poll Status, with Discharge Permitted


// Start Open-Wire ADC Conversions and Poll Status, with Discharge Permitted



/
//! @}

// Macros
//! Set "pin" low
//! @param pin pin to be driven LOW
#define output_low(pin)   digitalWrite(pin, LOW)
//! Set "pin" high
//! @param pin pin to be driven HIGH
#define output_high(pin)  digitalWrite(pin, HIGH)
//! Return the state of pin "pin"
//! @param pin pin to be read (HIGH or LOW).
//! @return the state of pin "pin"
#define input(pin)        digitalRead(pin)

//! @name ENDIAN DEPENDENT BYTE INDEXES
//! @{
//! Arduino/Linduino is a Little Endian Device, where the least significant byte is stored in the first byte of larger data types.
#ifdef BIG_ENDIAN
#define LSB 1 //!< Location of Least Signficant Byte when Word is accessed as Byte Array
#define MSB 0 //!< Location of Most Signficant Byte when Word is accessed as Byte Array
#define LSW 1 //!< Location of Least Signficant Word when Long Word is accessed as Byte Array
#define MSW 0 //!< Location of most Signficant Word when Long Word is accessed as Byte Array
#else
#define LSB 0 //!< Location of Least Signficant Byte when Word is accessed as Byte Array
#define MSB 1 //!< Location of Most Signficant Byte when Word is accessed as Byte Array
#define LSW 0 //!< Location of Least Signficant Word when Long Word is accessed as Byte Array
#define MSW 1 //!< Location of most Signficant Word when Long Word is accessed as Byte Array
#endif
//! @}

//! This union splits one int16_t (16-bit signed integer) or uint16_t (16-bit unsigned integer)
//! into two uint8_t's (8-bit unsigned integers) and vice versa.
  union LT_union_int16_2bytes
  {
    int16_t LT_int16;    //!< 16-bit signed integer to be converted to two bytes
    uint16_t LT_uint16;  //!< 16-bit unsigned integer to be converted to two bytes
    uint8_t LT_byte[2];  //!< 2 bytes (unsigned 8-bit integers) to be converted to a 16-bit signed or unsigned integer
  };

//! This union splits one int32_t (32-bit signed integer) or uint32_t (32-bit unsigned integer)
//! four uint8_t's (8-bit unsigned integers) and vice versa.
union LT_union_int32_4bytes
{
  int32_t LT_int32;       //!< 32-bit signed integer to be converted to four bytes
  uint32_t LT_uint32;     //!< 32-bit unsigned integer to be converted to four bytes
  uint8_t LT_byte[4];     //!< 4 bytes (unsigned 8-bit integers) to be converted to a 32-bit signed or unsigned integer
};

//! This union splits one int32_t (32-bit signed integer) or uint32_t (32-bit unsigned integer)
//! into two uint16_t's (16-bit unsigned integers) and vice versa.
union LT_union_uint32_2uint16s
{
  int32_t LT_int32;       //!< 32-bit signed integer to be converted to four bytes
  uint32_t LT_uint32;     //!< 32-bit unsigned integer to be converted to four bytes
  uint16_t LT_uint16[2];  //!< 2 words (unsigned 16-bit integers) to be converted to a 32-bit signed or unsigned integer
};

//! This union splits one float into four uint8_t's (8-bit unsigned integers) and vice versa.
union LT_union_float_4bytes
{
  float LT_float;      //!< float to be converted to four bytes
  uint8_t LT_byte[4];  //!< 4 bytes (unsigned 8-bit integers) to be converted to a float
};


#endif  // HARDWAREdESCRIPTION_H
