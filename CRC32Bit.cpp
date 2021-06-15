#include "CRC32Bit.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

//#define CRC_32 0x04C11DB7 //Ethernet, MPED-2, PNG, Gzip
//#define CRC_32 0xc9d204f5
//#define POLYNOMIAL CRC_32

bool CRC32Bit::verifyCRC(Packet* packet, uint32_t crc){
    int i, j;
    uint32_t _crc = doCRC(packet);
    return (crc==_crc);
}

void CRC32Bit::generateTable(uint32_t polynomial){
		for (uint32_t i = 0; i < 256; i++) {
			uint32_t c = i;
			for (size_t j = 0; j < 8; j++) {
				if (c & 1) {
					c = polynomial ^ (c >> 1);
				}
				else {
					c >>= 1;
				}
			}
			table[i] = c;
		}
}


uint32_t CRC32Bit::doCRC(Packet* packet){
    uint32_t crc = 0;
    uint8_t* u = new uint8_t[packet->getLength()];
    std::memcpy(u, packet->getData(), packet->getLength());//static_cast<const uint8_t*>(packet->getData());
    for (size_t i = 0; i < packet->getLength(); ++i) {
	crc = table[(crc ^ u[i]) & 0xFF] ^ (crc >> 8);
    }
    delete [] u;
    return crc;
}