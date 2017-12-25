/*!
 * Code to configure, set, and communicate with:
Linear Technology LTC6802G-1: Battery stack monitor

REVISION HISTORY
$Revision:  $
$Date: 2017-11-28 15:09:16 -0800 $

Copyright (c) 2017, Dr.Ing. Mohammed Nour A. Ahmed (mnahmed@eng.zu.edu.eg)
All rights reserved.
some parts of the code are adopted from Linear Technology Corp.(LTC) Linduino library:
http://www.linear.com/solutions/Linduino

@verbatim

NOTES
 Setup:
   Set the terminal baud rate to 115200 and select the newline terminator.
   Ensure all jumpers on the demo board are installed in their default positions from the factory.
   Refer to Demo Manual DC1651.


 Menu Entry 1: Write Configuration
   Writes the configuration register of the LTC6802s on the stack. This command can be used to turn on
   the reference and shorten ADC conversion Times.

 Menu Entry 2: Read Configuration
   Reads the configuration register of the LTC6802, the read configuration can differ from the written configuration.
   The GPIO pins will reflect the state of the pin

 Menu Entry 3: Start Cell voltage conversion
    Starts a LTC6802 cell channel adc conversion.

 Menu Entry 4: Read cell voltages
    Reads the LTC6802 cell voltage registers and prints the results to the serial port.

 Menu Entry 5: Start Auxiliary voltage conversion
    Starts a LTC6802 Temp channel adc conversion.

 Menu Entry 6: Read Auxiliary voltages
    Reads the LTC6802 Temp registers and prints the Temp voltages to the serial port.

 Menu Entry 7: Start cell voltage measurement loop
    The command will continuously measure the LTC6802 cell voltages and print the results to the serial port.
    The loop can be exited by sending the MCU a 'm' character over the serial link.

USER INPUT DATA FORMAT:
 decimal : 1024
 hex     : 0x400
 octal   : 02000  (leading 0)
 binary  : B10000000000
 float   : 1024.0
@endverbatim
 */


/*! @file
    @ingroup LTC68021
*/

#include <Arduino.h>
#include <stdint.h>
#include "hardwareDescription.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LTC68021.h"
#include <SPI.h>

const uint8_t TOTAL_IC = 1;//!<number of ICs in the daisy chain

/******************************************************
 *** Global Battery Variables received from 6803 commands
 These variables store the results from the LTC6802
 register reads and the array lengths must be based
 on the number of ICs on the stack
 ******************************************************/
uint16_t cell_codes[TOTAL_IC][12];
/*!<
  The cell codes will be stored in the cell_codes[][12] array in the following format:

  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|    .....     |  cell_codes[0][11]|  cell_codes[1][0] | cell_codes[1][1]|  .....   |
  |------------------|------------------|------------------|--------------|-------------------|-------------------|-----------------|----------|
  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |    .....     |  IC1 Cell 12      |IC2 Cell 1         |IC2 Cell 2       | .....    |
****/

uint16_t temp_codes[TOTAL_IC][3];
/*!<
 The Temp codes will be stored in the temp_codes[][3] array in the following format:

 |  temp_codes[0][0]| temp_codes[0][1]|temp_codes[0][2] | temp_codes[1][0]| temp_codes[1][1]|   .....   |
 |------------------|-----------------|-----------------|-----------------|-----------------|-----------|
 |IC1 Temp1         |IC1 Temp2        |IC1 ITemp        |IC2 Temp1        |IC2 Temp2        |  .....    |
*/

uint8_t tx_cfg[TOTAL_IC][6];
/*!<
  The tx_cfg[][6] stores the LTC6802 configuration data that is going to be written
  to the LTC6802 ICs on the daisy chain. The LTC6802 configuration data that will be
  written should be stored in blocks of 6 bytes. The array should have the following format:

 |  tx_cfg[0][0]| tx_cfg[0][1] |  tx_cfg[0][2]|  tx_cfg[0][3]|  tx_cfg[0][4]|  tx_cfg[0][5]| tx_cfg[1][0] |  tx_cfg[1][1]|  tx_cfg[1][2]|  .....    |
 |--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|-----------|
 |IC1 CFGR0     |IC1 CFGR1     |IC1 CFGR2     |IC1 CFGR3     |IC1 CFGR4     |IC1 CFGR5     |IC2 CFGR0     |IC2 CFGR1     | IC2 CFGR2    |  .....    |

*/

uint8_t rx_cfg[TOTAL_IC][7];
/*!<
  the rx_cfg[][8] array stores the data that is read back from a LTC6802-1 daisy chain.
  The configuration data for each IC  is stored in blocks of 7 bytes. Below is an table illustrating the array organization:

|rx_config[0][0]|rx_config[0][1]|rx_config[0][2]|rx_config[0][3]|rx_config[0][4]|rx_config[0][5]|rx_config[0][6]  |rx_config[1][0]|rx_config[1][1]|  .....    |
|---------------|---------------|---------------|---------------|---------------|---------------|-----------------|---------------|---------------|----------|
|IC1 CFGR0      |IC1 CFGR1      |IC1 CFGR2      |IC1 CFGR3      |IC1 CFGR4      |IC1 CFGR5      |IC1 PEC          |IC2 CFGR0      |IC2 CFGR1      |  .....    |
*/


//! Inititializes hardware and variables
void setup()
{
  Serial.begin(115200);
  LTC6802_initialize(); //Initialize LTC6802 hardware
  init_cfg();        	//initialize the 6802 configuration array to be written
  print_menu();
}


//!main loop
void loop()
{

  if (Serial.available()){          // Check for user input
    uint32_t user_command;
    user_command = read_int();      // Read the user command
    Serial.println(user_command);
    run_command(user_command);
  }
}


/*!*****************************************
  \brief executes the user inputted command

  Menu Entry 1: Write Configuration \n
   Writes the configuration register of the LTC6802. This command can be used to turn on the reference
   and increase the speed of the ADC conversions.

 Menu Entry 2: Read Configuration \n
   Reads the configuration register of the LTC6802, the read configuration can differ from the written configuration.
   The GPIO pins will reflect the state of the pin

 Menu Entry 3: Start Cell voltage conversion \n
   Starts a LTC6802 cell channel adc conversion.

 Menu Entry 4: Read cell voltages \n
    Reads the LTC6802 cell voltage registers and prints the results to the serial port.

 Menu Entry 5: Start Temp voltage conversion \n
    Starts a LTC6802 Temp channel adc conversion.

 Menu Entry 6: Read Temp voltages \n
    Reads the LTC6802 axiliary registers and prints the GPIO voltages to the serial port.

 Menu Entry 7: Start cell voltage measurement loop \n
    The command will continuously measure the LTC6802 cell voltages and print the results to the serial port.
    The loop can be exited by sending the MCU a 'm' character over the serial link.

*******************************************/
void run_command(uint32_t cmd)
{
  int8_t error = 0;

  char input = 0;
  switch (cmd){
    case 1:
      LTC6802_wrcfg(TOTAL_IC,tx_cfg);
      print_config();
      break;

    case 2:
      error = LTC6802_rdcfg(TOTAL_IC,rx_cfg);
      if (error == -1){
        Serial.println(F("A PEC error was detected in the received data"));
      }
      print_rxconfig();
      break;

    case 3:
      LTC6802_stcvad();
      delay(3);
      Serial.println(F("cell conversion completed"));
      Serial.println();
      break;

    case 4:
      error = LTC6802_rdcv(TOTAL_IC,cell_codes); // Set to read back all cell voltage registers
      if (error == -1){
        Serial.println(F("A PEC error was detected in the received data"));
      }
      print_cells();
      break;

    case 5:
      LTC6802_sttmpad();
      delay(3);
      Serial.println(F("aux conversion completed"));
      Serial.println();
      break;

    case 6:
      error = LTC6802_rdtmp(TOTAL_IC,temp_codes); // Set to read back all aux registers
      if (error == -1){
        Serial.println(F("A PEC error was detected in the received data"));
      }
      print_temp();
      break;

    case 7:
      Serial.println(F("transmit 'm' to quit"));

      LTC6802_wrcfg(TOTAL_IC,tx_cfg);
      while (input != 'm'){
        if (Serial.available() > 0){
          input = read_char();
        }


        LTC6802_stcvad();
        delay(10);

        error = LTC6802_rdcv(TOTAL_IC,cell_codes);
        if (error == -1){
          Serial.println(F("A PEC error was detected in the received data"));
        }
        print_cells();


        if (error == -1){
          Serial.println(F("A PEC error was detected in the received data"));
        }
        // print_rxconfig();

        delay(500);
      }
      print_menu();
      break;

    default:
      Serial.println(F("Incorrect Option"));
      break;
  }
}


//! Initializes the configuration array
void init_cfg()
{
  for (int i = 0; i<TOTAL_IC; i++){
    tx_cfg[i][0] = 0xF1;
    tx_cfg[i][1] = 0x00 ;
    tx_cfg[i][2] = 0x00 ;
    tx_cfg[i][3] = 0x00 ;
    tx_cfg[i][4] = 0x00 ;
    tx_cfg[i][5] = 0x00 ;
  }
}


//! Prints the main menu
void print_menu()
{
  Serial.println(F("Please enter LTC6802LTC6802 Command"));
  Serial.println(F("1: Write Configuration"));
  Serial.println(F("2: Read Configuration"));
  Serial.println(F("3: Start Cell Voltage Conversion"));
  Serial.println(F("4: Read Cell Voltages"));
  Serial.println(F("5: Start Aux Voltage Conversion"));
  Serial.println(F("6: Read Aux Voltages"));
  Serial.println(F("7: loop cell voltages"));
  Serial.println(F("Please enter command number: "));
  Serial.println();
}




//!Prints cell coltage codes to the serial port
void print_cells()
{
  for (int current_ic = 0 ; current_ic < TOTAL_IC; current_ic++){
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    for (int i=0; i<12; i++)
    {
      Serial.print(" C");
      Serial.print(i+1,DEC);
      Serial.print(":");
      Serial.print(cell_codes[current_ic][i]*0.0015,4);
      Serial.print(",");
    }
    Serial.println();
  }
  Serial.println();
}


//!Prints GPIO voltage codes and Vref2 voltage code onto the serial port
void print_temp()
{
  for (int current_ic =0 ; current_ic < TOTAL_IC; current_ic++){
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    for (int i=0; i < 2; i++){
      Serial.print(" Temp-");
      Serial.print(i+1,DEC);
      Serial.print(":");
      Serial.print(temp_codes[current_ic][i]*0.0015,4);
      Serial.print(",");
    }
    Serial.print(" ITemp");
    Serial.print(":");
    Serial.print((temp_codes[current_ic][2]*0.1875)-274.15,4);
    Serial.println();
  }
  Serial.println();
}

//!Prints the configuration data that is going to be written to the LTC6802 to the serial port.
void print_config()
{
  int cfg_pec;

  Serial.println("Written Configuration: ");
  for (int current_ic = 0; current_ic<TOTAL_IC; current_ic++){
    Serial.print(" IC ");   Serial.print(current_ic+1,DEC);
    Serial.print(": ");     
    Serial.print("0x");     serial_print_hex(tx_cfg[current_ic][0]);
    Serial.print(", 0x");   serial_print_hex(tx_cfg[current_ic][1]);
    Serial.print(", 0x");   serial_print_hex(tx_cfg[current_ic][2]);
    Serial.print(", 0x");   serial_print_hex(tx_cfg[current_ic][3]);
    Serial.print(", 0x");   serial_print_hex(tx_cfg[current_ic][4]);
    Serial.print(", 0x");   serial_print_hex(tx_cfg[current_ic][5]);
    Serial.print(", Calculated PEC: 0x");    
    cfg_pec = pec8_calc(6,&tx_cfg[current_ic][0]);
                            serial_print_hex((uint8_t)(cfg_pec>>8));
    Serial.print(", 0x");   serial_print_hex((uint8_t)(cfg_pec));
    Serial.println();
  }
  Serial.println();
}


//! Prints the configuration data that was read back from the LTC6802 to the serial port.
void print_rxconfig()
{
  Serial.println("Received Configuration ");
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++){
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    Serial.print(": 0x");               serial_print_hex(rx_cfg[current_ic][0]);
    Serial.print(", 0x");               serial_print_hex(rx_cfg[current_ic][1]);
    Serial.print(", 0x");               serial_print_hex(rx_cfg[current_ic][2]);
    Serial.print(", 0x");               serial_print_hex(rx_cfg[current_ic][3]);
    Serial.print(", 0x");               serial_print_hex(rx_cfg[current_ic][4]);
    Serial.print(", 0x");               serial_print_hex(rx_cfg[current_ic][5]);
    Serial.print(", Received PEC: 0x"); serial_print_hex(rx_cfg[current_ic][6]);

    Serial.println();
  }
  Serial.println();
}

//! Prints Data in a Hex Format
void serial_print_hex(uint8_t data)
{
  if (data < 16){
    Serial.print("0");
    Serial.print((byte)data,HEX);
  }else{
    Serial.print((byte)data,HEX);
  }
}
