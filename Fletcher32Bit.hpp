#ifndef FLETCHER32BIT_HPP_INCLUDED
#define FLETCHER32BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"
#include <string>

class Fletcher32Bit: public VerificationAlgorithm {
private:
    uint32_t initialC1;
    uint32_t initialC2;
    uint32_t modulusVal;

public:
    Fletcher32Bit(bool is1sComp);
    bool verifyChecksum(Packet* packet, uint32_t chk);
    uint32_t doChecksum(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return doChecksum(packet);
    }
    std::string getAlgName(){
        if (this->modulusVal == 65535) return "1CFL32";
        else return "2CFL32";
        
    }
};

#endif
