#ifndef CRC16BIT_HPP_INCLUDED
#define CRC16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

class CRC16Bit: public VerificationAlgorithm {
private:
    uint16_t crc16;
public:
    public:
    CRC16Bit(uint16_t crc16_){
        crc16 = crc16_;
    }
    bool verifyCRC(Packet* packet, uint16_t chk);
    uint16_t doCRC(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return (uint32_t) doCRC(packet);
    }
};

#endif
