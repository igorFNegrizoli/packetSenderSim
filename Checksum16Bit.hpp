#ifndef CHECKSUM16BIT_HPP_INCLUDED
#define CHECKSUM16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

class Checksum16Bit: public VerificationAlgorithm {
    public:
    bool verifyChecksum(Packet* packet, uint16_t chk);
    uint16_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return (uint32_t) doChecksum(packet);
    }
};

#endif
