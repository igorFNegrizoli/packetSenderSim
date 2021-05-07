#ifndef CRC16BIT_HPP_INCLUDED
#define CRC16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"

class CRC16Bit {
public:
    public:
    bool verifyCRC(Packet* packet, uint16_t chk);
    uint16_t doCRC(Packet* packet);
    
};

#endif
