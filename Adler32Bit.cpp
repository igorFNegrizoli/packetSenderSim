#include "Adler32Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>

/*
(0,0,65535) for ones complement fletcher
(0,0,65536) for twos complement fletcher
(1,0,65521) for adler
*/
Adler32Bit::Adler32Bit(){
    this->initialC1 = 1;
    this->initialC2 = 0;
    this->modulusVal = 65521;
}

uint32_t Adler32Bit::doChecksum(Packet* packet){ 
    uint8_t* data = (uint8_t*)packet->getData();
    uint32_t c1 = this->initialC1;
    uint32_t c2 = this->initialC2;

    for(int i=0; i < packet->getLength(); i++){
        c1 = (c1+data[i])%this->modulusVal;
        c2 = (c2+c1)%this->modulusVal;
        //std::cout << "c1: " << c1 << " - c2: " << c2 << std::endl;
    }

    return (uint32_t)(c1 << 16)+(uint16_t)c2;
}
