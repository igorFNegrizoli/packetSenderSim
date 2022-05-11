#ifndef FLETCHERADLER32BIT_HPP_INCLUDED
#define FLETCHERADLER32BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

class FletcherAdler32Bit: public VerificationAlgorithm {
private:
    uint32_t initialC1;
    uint32_t initialC2;
    uint32_t modulusVal;

public:
    FletcherAdler32Bit(uint32_t initialC1_, uint32_t initialC2_, uint32_t modulusVal_);
    bool verifyChecksum(Packet* packet, uint32_t chk);
    uint32_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return doChecksum(packet);
    }
};

#endif
