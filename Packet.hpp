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
	RNG* rng;
    public:
        Packet();
	Packet(RNG *rng);
        Packet(uint16_t len);
	Packet(uint16_t len, RNG *rng);
        Packet(uint16_t len, bool zeroOrOne);

	uint8_t* getData();
        uint16_t getLength();

	Packet* clone();

        void print(char mode);

	//retornam quantos bits foram afetados
        uint16_t injectErrorInChunk(uint64_t bit_pos);
        uint16_t injectErrorInChunk(uint64_t bit_start, uint64_t bit_end);

        ~Packet();
};
#endif
