/*
 * This code implements a slave side in SPI communication
 * to verify that the data was recieved successfully from
 * the master unit.
 * 
 * This program simply works as a slave and print out 
 * any recieved data.
 * 
 * To use this code you just need to connect the following pins:
 *    Master_side     Salve_side
 *    pin 13----------pin 13    'SCK'
 *    pin 12----------pin 12    'MISO'
 *    pin 11----------pin 11    'MOSI'
 *    pin 10----------pin 10    'SS'
 *    
 * The source code is taken from arduino forum:
 * https://forum.arduino.cc/index.php?topic=120049.0
 * 
 * 
 */

#include "Spi.h"
int i=0;
#define rx_size 100
#define tx_size 100
volatile uint8_t rx[rx_size];
volatile uint8_t tx[tx_size];
volatile boolean Flag = LOW;

void setup(){
  Serial.begin(115200);
  Spi.SPI_SlaveInit();
  init_tx();
}

void loop(){
  /* if SS_PIN is LOW, then, there is data to receive */
  if(digitalRead(SS_PIN) == LOW){
      while(digitalRead(SS_PIN) == LOW){
      
      rx[i++] = Spi.SPI_SlaveReceive(i);   // receiving the next byte
//      if(i<(rx_size-1)){   // checking for upper bound for array index of size 100
//        i++;  // increasing pointer to store upcoming byte in
//      }
    }
    Flag = HIGH;    // raising flag to print the received data
  }
  
  
  if(Flag){       // if Flag is HIGH, then, there is new data received
    Flag = LOW;   // resetting falg to LOW to avoid printing same data
    Serial.println("Received Data:");  
    print_rx();   // a function to print the received array of bytes in HEX format
    i = 0;        // resetting the index pointer to the first location 'index 0'
  }

/* part of code to test the operation of PEC calculation function*/
  if(Serial.available()){
    uint8_t c[1];
    c[0] = Serial.parseInt();
    Serial.print("PEC of 0x");
    Serial.print(c[0], HEX);
    Serial.print(" is: 0x");
    Serial.println(calculatePEC(1, c), HEX);
    while(Serial.available()){
      Serial.read();
    }
  }
}

void print_rx(){
  for (int x=0 ; x<i ; x++){    // variable i holds the number of received bytes
    Serial.print(rx[x], HEX);   // printing the byte in HEX format
    Serial.print(",  ");
  }
  Serial.println();
  Serial.print("------------- ");
  Serial.print(Spi.cmd_counter);
  Serial.println(" -------------");
}

void init_tx(){
  for(int n=0 ; n<tx_size ; n++ ){
    tx[n] = n;
  }
}

