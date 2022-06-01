#include "CRC16Bit.hpp"
#include <iostream>
#include <cmath>

bool CRC16Bit::verifyCRC(Packet* packet, uint16_t crc){
    return (crc == doCRC(packet));
}

uint16_t CRC16Bit::doCRC(Packet* packet){
    uint16_t crc = 0xffff;
    int count = packet->getLength();
    uint8_t* ptr = (uint8_t*)packet->getData();

    while(--count >= 0 )
    {
        crc = crc ^ ((uint16_t) (*ptr++ << 8));
        for(int i = 0; i < 8; i++)
        {
            if( crc & 0x8000 )
            {
                crc = (crc << 1) ^ this->crc16;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}
