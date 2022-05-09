#include "CRC32Bit.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

//#define CRC_32 0x04C11DB7 //Ethernet, MPED-2, PNG, Gzip
//#define CRC_32 0xc9d204f5
//#define POLYNOMIAL CRC_32

CRC32Bit::CRC32Bit(uint32_t polynomial_){
	polynomial = polynomial_;
	generateTable();
}

bool CRC32Bit::verifyCRC(Packet* packet, uint32_t crc){
    return crc==doCRC(packet);
}

void CRC32Bit::generateTable(){
	for (unsigned int byte = 0; byte <= 0xFF; byte++){
		uint32_t crc = (byte << 24);
		for (int bit = 0; bit < 8; bit++ ){
			if (crc & (1L << 31)) crc = (crc << 1) ^ this->polynomial;
				else crc = (crc << 1);
			}
		table[ byte ] = crc;
	}
}


uint32_t CRC32Bit::doCRC(Packet* packet){
    //uint32_t crc = 0;
    //uint8_t* u = new uint8_t[packet->getLength()];
    uint8_t* data8B = packet->getData();
    uint16_t len = packet->getLength();
    //std::memcpy(u, packet->getData(), packet->getLength());//static_cast<const uint8_t*>(packet->getData());

    uint32_t crc = -1;
	while( len-- )
		crc = table[ (*data8B++  ^ (crc >> 24)) & 0xFF ] ^ (crc << 8);
	return ~crc;
    //delete [] u;
    //return crc;
}