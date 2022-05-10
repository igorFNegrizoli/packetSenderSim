#include "CRC16Bit.hpp"
#include <iostream>
#include <cmath>

bool CRC16Bit::verifyCRC(Packet* packet, uint16_t crc){
    return (crc == doCRC(packet));
}

uint16_t CRC16Bit::doCRC(Packet* packet){
    int i, j;
    uint16_t crc = 0;
    uint16_t* data16B = (uint16_t*)packet->getData();

    for (i = 0; i < packet->getLength()/2; i++){
        crc ^= data16B[i];
        for (j = 0; j < 16; j++){
            if (crc & 1) crc ^= crc16;
            crc >>= 1;
        }
    }
    return crc;
}
