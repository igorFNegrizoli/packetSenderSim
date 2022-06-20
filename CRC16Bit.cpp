#include "CRC16Bit.hpp"
#include <iostream>
#include <cmath>

bool CRC16Bit::verifyCRC(Packet* packet, uint16_t crc){
    return (crc == doCRC(packet));
}

/* crc16Bootstrap() is only used to bootstrap an initial 256-entry lookup table. */
uint16_t crc16Bootstrap(uint16_t crc, const void *in_data, uint64_t len, uint16_t crc16) {
    const uint8_t *data = (uint8_t*)in_data;
    for (uint64_t i = 0; i < len; i++) {
        crc = crc ^ (data[i] << 8);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ crc16;
            } else {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}

void CRC16Bit::crcspeed16little_init(crcfn16 crcfn, uint16_t table[8][256]) {
    uint16_t crc;

    /* generate CRCs for all single byte sequences */
    for (int n = 0; n < 256; n++) {
        table[0][n] = crcfn(0, &n, 1, this->crc16);
    }

    /* generate nested CRC table for future slice-by-8 lookup */
    for (int n = 0; n < 256; n++) {
        crc = table[0][n];
        for (int k = 1; k < 8; k++) {
            crc = table[0][(crc >> 8) & 0xff] ^ (crc << 8);
            table[k][n] = crc;
        }
    }
}

uint16_t CRC16Bit::crc16_lookup(uint16_t crc, const void *in_data, uint64_t len) {
    const uint8_t *data = (uint8_t*)in_data;
    for (uint64_t i = 0; i < len; i++) {
        crc = (crc << 8) ^ crc16_table[0][((crc >> 8) ^ data[i]) & 0x00ff];
    }

    return crc;
}

uint16_t CRC16Bit::doCRC(Packet* packet){
    if(!isInit){
        crcspeed16little_init(crc16Bootstrap, crc16_table);
        isInit = true;
    }
    return crc16_lookup(0, packet->getData(), packet->getLength());
}
