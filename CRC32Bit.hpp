#ifndef CRC32BIT_HPP_INCLUDED
#define CRC32BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"
#include "VerificationAlgorithm.hpp"

class CRC32Bit: public VerificationAlgorithm {
    private:
      uint16_t table[256];
    public:
      void generateTable(uint32_t polynomial);
      bool verifyCRC(Packet* packet, uint32_t chk);
      uint32_t doCRC(Packet* packet);    
      uint32_t generateVerificationCode(Packet* packet){
        return doCRC(packet);
    }
};

#endif
