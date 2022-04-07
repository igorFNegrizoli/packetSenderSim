#ifndef CRC16BIT_HPP_INCLUDED
#define CRC16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

class CRC16Bit: public VerificationAlgorithm {
public:
    public:
    bool verifyCRC(Packet* packet, uint16_t chk);
    uint16_t doCRC(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return (uint32_t) doCRC(packet);
    }
};

#endif
