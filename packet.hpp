#ifndef PACKET_HPP_INCLUDED
#define PACKET_HPP_INCLUDED
#include <cstdint>

//Implementação em classes mais concisa e legivel que a a versão anterior

class packet{
        /*The length field is used to store the original length in the case the len field is modified
        by the error model. The checksum field also stores only the original checksum, the
        */
    protected:
        uint16_t length, checksum;
        uint16_t* packetData;
    public:
        packet();
        packet(uint16_t len);
        packet(uint16_t len, bool zeroOrOne);
        uint16_t getLength();
        uint16_t getChecksum();
        void setLength(uint16_t a);
        void setChecksum(uint16_t a);
        bool verifyChecksum();
        void setChecksum();
        void printPacket(char mode);
        void injectErrorInChunk(uint16_t pos, uint16_t microPosition);
        void bernoulliModel(double BER);
        void burstErrorPeriodicModel(int16_t Tmin, uint16_t Tmax, uint16_t Nmin, uint16_t Nmax);
        void gilbertModel(uint16_t burst, double plRate);

        virtual uint16_t doChecksum(uint16_t* packet, uint16_t length){
            
            return 0;
        }

        ~packet();
};

#endif
