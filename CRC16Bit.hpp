#ifndef CRC16BIT_HPP_INCLUDED
#define CRC16BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

typedef uint16_t (*crcfn16)(uint16_t, const void *, const uint64_t, uint16_t);

class CRC16Bit: public VerificationAlgorithm {
private:
    uint16_t crc16;
    bool isInit;
    uint16_t crc16_table[8][256] = {{0}};
    void crcspeed16little_init(crcfn16 crcfn, uint16_t table[8][256]);
    uint16_t crc16_lookup(uint16_t crc, const void *in_data, uint64_t len);

public:
    public:
    CRC16Bit(uint16_t crc16_){
        crc16 = crc16_;
        isInit = false;
    }
    bool verifyCRC(Packet* packet, uint16_t chk);
    uint16_t doCRC(Packet* packet);
    uint32_t generateVerificationCode(Packet* packet){
        return (uint32_t) doCRC(packet);
    }
    std::string getAlgName(){
        return "CRC16";
    }
};

#endif
