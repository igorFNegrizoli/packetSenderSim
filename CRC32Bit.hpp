#ifndef CRC32BIT_HPP_INCLUDED
#define CRC32BIT_HPP_INCLUDED
#include <cstdint>
#include "Packet.hpp"

class CRC32Bit {
    private:
      uint16_t table[256];
    public:
      void generateTable();
      bool verifyCRC(Packet* packet, uint32_t chk);
      uint32_t doCRC(Packet* packet);    
};

#endif
