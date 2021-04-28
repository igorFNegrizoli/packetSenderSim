#ifndef CHECKSUM16BIT_HPP_INCLUDED
#define CHECKSUM16BIT_HPP_INCLUDED
#include <cstdint>
#include "packet.hpp"

class checksum16Bit:public packet{
    public:
    checksum16Bit();
    checksum16Bit(uint16_t len);
    checksum16Bit(uint16_t len, bool zeroOrOne);
    bool verifyChecksum();
    uint16_t doChecksum(uint16_t* packet, uint16_t length);
    ~checksum16Bit();
    
};

#endif