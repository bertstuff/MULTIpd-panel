/**
 * \addtogroup Type
 * @{
 */

/**
 * \defgroup Float_ieee754
 *
 * converter for ieee754 format
 *
 * @{
 */

/**
 * \file
 * This file contains functions to convert ieee745 to float and backwards 
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
  #ifndef _RFID_MicroRWD_
  #define _RFID_MicroRWD_ 
  
  #include <stdint.h>

  #define pack754_32(f) (pack754((f), 32, 8))
  #define pack754_64(f) (pack754((f), 64, 11))
  #define unpack754_32(i) (unpack754((i), 32, 8))
  #define unpack754_64(i) (unpack754((i), 64, 11))

  uint64_t pack754(long double f, unsigned bits, unsigned expbits);

  long double unpack754(uint64_t i, unsigned bits, unsigned expbits);
  
  #endif 
  /**
 * 	@}
 * @}
 **/
