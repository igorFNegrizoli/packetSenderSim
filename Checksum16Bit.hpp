#ifndef CHECKSUM16BIT_HPP_INCLUDED
#define CHECKSUM16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"

class Checksum16Bit {
    public:
    bool verifyChecksum(Packet* packet, uint16_t chk);
    uint16_t doChecksum(Packet* packet);
};

#endif
