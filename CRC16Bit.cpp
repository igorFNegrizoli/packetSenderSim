#include "CRC16Bit.hpp"
#include <iostream>
#include <cmath>

#define CRC_16_CCITT 0x1021
#define POLYNOMIAL CRC_16_CCITT

bool CRC16Bit::verifyCRC(Packet* packet, uint16_t crc){
    int i, j;
    uint16_t _crc = doCRC(packet);
    return (crc==_crc);
}

uint16_t CRC16Bit::doCRC(Packet* packet){

    int i, j;
    uint16_t crc = 0;

    for (i = 0; i < packet->getLength()/2; i++){
        crc ^= packet->getData()[i];
        for (j = 0; j < 16; j++){
            if (crc & 1) crc ^= POLYNOMIAL;
            crc >>= 1;
        }
    }
    return crc;
}
