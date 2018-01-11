/*  To be added:
 *    1. function to read all cells
 *    2. function to read specific cell
 *    3. function to get number of batteries
 *    
 */ 

void readAllCells(uint8_t IC_NUM){
  /*  
   * 1. send start_ADC_conversion command
   * 2. send read_cell_voltage command
   * 3. map values from uint16 to float
   * 4. update CellVoltage global variables
   */
}

float readCell(uint8_t IC_NUM, uint8_t CellNum){
  float cv = 0.0;
  /*  
   * 1. call readAllCells routine
   * 2. return the required cell voltage
   */
  return cv;
}

void readTemperature(uint8_t IC_NUM){
  /*  
   * 1. send start_ADC_conversion command
   * 2. send read_temp command
   * 3. map values from uint16 to float
   * 4. update Temperature global variables
   */
}

uint8_t getNumOfBatteries(){
  uint8_t N;
  /*  
   * To be implemented later
   */
  return N;
}

