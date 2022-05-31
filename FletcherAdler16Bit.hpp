#ifndef FLETCHERADLER16BIT_HPP_INCLUDED
#define FLETCHERADLER16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"
#include <string>

class FletcherAdler16Bit: public VerificationAlgorithm {
private:
    uint16_t initialC1;
    uint16_t initialC2;
    uint16_t modulusVal;

public:
    FletcherAdler16Bit(uint16_t initialC1_, uint16_t initialC2_, uint16_t modulusVal_);
    //bool verifyChecksum(Packet* packet, uint16_t chk);
    uint16_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return (uint32_t) doChecksum(packet);
    }
    std::string getAlgName(){
        return "FLAD16";
    }
};

#endif
