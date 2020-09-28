#ifndef __CRC16_CCITT_H__
#define __CRC16_CCITT_H__

uint16_t crc16ccitt(uint8_t const *message, int nBytes);

uint16_t crc16ccitt_xmodem(uint8_t const *message, int nBytes);
uint16_t crc16ccitt_xmodem_remainder(uint8_t const *message, int nBytes, uint16_t remainder);

uint16_t crc16ccitt_kermit(uint8_t const *message, int nBytes);

uint16_t crc16ccitt_1d0f(uint8_t const *message, int nBytes);

uint16_t crc16ibm(uint8_t const *message, int nBytes);

#endif /* __CRC16_CCITT_H__ */

/** @} */
/** @} */
