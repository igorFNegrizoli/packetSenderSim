#ifndef Included_Packet_HPP
#define Included_Packet_HPP

#include <cstdint>
#include <iostream>
#include "RNG.hpp"

//Implementação em classes mais concisa e legivel que a a versão anterior

class Packet{
    private:
        uint16_t length;
        uint8_t* data;
        uint16_t* data16b;
        uint32_t* data32b;
	RNG* rng;
    public:
        Packet();
        Packet(RNG *rng);
        Packet(uint16_t len);
        Packet(uint16_t len, RNG *rng);
        Packet(uint16_t len, bool zeroOrOne);
        void* getData(uint16_t len = 1);
        uint16_t getLength();
        Packet* clone();
        void print(char mode);
        void updateBuffers();
        void* char2BigEndian(uint16_t wordSize);
        //retornam quantos bits foram afetados
        uint16_t injectErrorInChunk(uint64_t bit_pos);
        //uint16_t injectErrorInChunk(uint64_t bit_start, uint64_t bit_end);
        ~Packet();
};
#endif
