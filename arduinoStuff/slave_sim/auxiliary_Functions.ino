uint8_t calculatePEC(uint8_t len, uint8_t *data){
  uint8_t  remainder = 0x41;  //PEC_SEED;

// Perform modulo-2 division, a byte at a time.
  for (int byte = 0; byte < len; ++byte){

// Bring the next byte into the remainder.
    remainder ^= data[byte];

// Perform modulo-2 division, a bit at a time.
    for (uint8_t bit = 8; bit > 0; --bit){

// Try to divide the current data bit.
      if (remainder & 128){
        remainder = (remainder << 1) ^ 7/*PEC_POLY*/;
      }else{
        remainder = (remainder << 1);
      }
    }
  }
  return remainder;
}
