#ifndef ADLER32BIT_HPP_INCLUDED
#define ADLER32BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"
#include <string>

class Adler32Bit: public VerificationAlgorithm {
private:
    uint32_t initialC1;
    uint32_t initialC2;
    uint32_t modulusVal;

public:
    Adler32Bit();
    bool verifyChecksum(Packet* packet, uint32_t chk);
    uint32_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return doChecksum(packet);
    }
    std::string getAlgName(){
        return "ADLR32";
    }
};

#endif
