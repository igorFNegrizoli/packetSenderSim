#ifndef CHECKSUM32BIT_HPP_INCLUDED
#define CHECKSUM32BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

class Checksum32Bit: public VerificationAlgorithm {
    public:
    bool verifyChecksum(Packet* packet, uint32_t chk);
    uint32_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return doChecksum(packet);
    }
};

#endif
