#include <stdint.h>

uint16_t straight_16(uint16_t value) {
    return value;
}

uint16_t reverse_16(uint16_t value) {
	uint16_t i;
	uint16_t reversed = 0;
    for (i = 0; i < 16; ++i) {
        reversed <<= 1;
        reversed |= value & 0x1;
        value >>= 1;
    }
    return reversed;
}

uint8_t straight_8(uint8_t value) {
    return value;
}

uint8_t reverse_8(uint8_t value) {
    uint16_t i;
	uint8_t reversed = 0;
    for (i = 0; i < 8; ++i) {
        reversed <<= 1;
        reversed |= value & 0x1;
        value >>= 1;
    }
    return reversed;
}

uint16_t crc16(uint8_t const *message, int nBytes, uint8_t(*data_order)(uint8_t value), uint16_t(*remainder_order)(uint16_t value), uint16_t remainder, uint16_t polynomial) {
	uint8_t bit;
	uint16_t byte;
    for (byte = 0; byte < nBytes; ++byte) {
        remainder ^= (data_order(message[byte]) << 8);
        for (bit = 8; bit > 0; --bit) {
            if (remainder & 0x8000) {
                remainder = (remainder << 1) ^ polynomial;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder_order(remainder);
}


uint16_t crc16ccitt(uint8_t const *message, int nBytes) {
    return crc16(message, nBytes, straight_8, straight_16, 0xffff, 0x1021);
}

uint16_t crc16ccitt_xmodem(uint8_t const *message, int nBytes) {
    return crc16(message, nBytes, straight_8, straight_16, 0x0000, 0x1021);
}

uint16_t crc16ccitt_xmodem_remainder(uint8_t const *message, int nBytes, uint16_t remainder) {
    return crc16(message, nBytes, straight_8, straight_16, remainder, 0x1021);
}

uint16_t crc16ccitt_kermit(uint8_t const *message, int nBytes) {
    uint16_t swap = crc16(message, nBytes, reverse_8, reverse_16, 0x0000, 0x1021);
    return swap << 8 | swap >> 8;
}

uint16_t crc16ccitt_1d0f(uint8_t const *message, int nBytes) {
    return crc16(message, nBytes, straight_8, straight_16, 0x1d0f, 0x1021);
}

uint16_t crc16ibm(uint8_t const *message, int nBytes) {
    return crc16(message, nBytes, reverse_8, reverse_16, 0x0000, 0x8005);
}

