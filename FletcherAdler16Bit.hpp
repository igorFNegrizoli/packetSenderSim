#ifndef FLETCHERADLER16BIT_HPP_INCLUDED
#define FLETCHERADLER16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"
#include <string>

class FletcherAdler16Bit: public VerificationAlgorithm {
protected:
    uint16_t initialC1;
    uint16_t initialC2;
    uint16_t modulusVal;

public:
    FletcherAdler16Bit(){}
    FletcherAdler16Bit(uint16_t initialC1_, uint16_t initialC2_, uint16_t modulusVal_);
    //bool verifyChecksum(Packet* packet, uint16_t chk);
    uint16_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return (uint32_t) doChecksum(packet);
    }
    std::string getAlgName(){
        if(this->modulusVal == 255) return "1CFL16";
        else if(this->modulusVal == 256) return "2CFL16";
        else return "ADLR16";
    }
};

#endif
