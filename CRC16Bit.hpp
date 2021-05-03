#ifndef CRC16BIT_HPP_INCLUDED
#define CRC16BIT_HPP_INCLUDED
#include <cstdint>
#include "packet.hpp"

class CRC16Bit:public packet{
private:
	uint16_t firstOnePos;
	uint16_t firstOneMicroPos;
	uint16_t poly;
public:
    CRC16Bit();
    CRC16Bit(uint16_t len);
    CRC16Bit(uint16_t len, bool zeroOrOne);
    CRC16Bit(packet* basePacket);
    bool verifyChecksum();
    uint16_t doChecksum(uint16_t* packet, uint16_t poly);
    ~CRC16Bit();
    
};

#endif