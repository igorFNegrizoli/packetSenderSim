#ifndef UDP_H_INCLUDED
#define UDP_H_INCLUDED
#include <cstdint>

//Implementação em classes mais concisa e legivel que a a versão anterior

class udpPacket{
        uint16_t length, checksum;
        uint16_t* packet;
    public:
        udpPacket();
        udpPacket(uint16_t len);
        uint16_t getLength();
        uint16_t getChecksum();
        void setLength(uint16_t a);
        void setChecksum(uint16_t a);
        bool verifyChecksum();
        void setChecksum();
        void printPacket();
        ~udpPacket();
};

#endif