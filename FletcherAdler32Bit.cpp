#include "FletcherAdler32Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>

/*
(0,0,65535) for ones complement fletcher
(0,0,65536) for twos complement fletcher
(1,0,65521) for adler
*/
FletcherAdler32Bit::FletcherAdler32Bit(uint32_t initialC1_, uint32_t initialC2_, uint32_t modulusVal_){
    this->initialC1 = initialC1_;
    this->initialC2 = initialC2_;
    this->modulusVal = modulusVal_;
}

uint32_t FletcherAdler32Bit::doChecksum(Packet* packet){ 
    uint16_t* data = (uint16_t*)packet->getData(2);
    uint32_t c1 = this->initialC1;
    uint32_t c2 = this->initialC2;

    for(int i=0; i < packet->getLength()/2; i++){
        c1 = (c1+data[i])%this->modulusVal;
        c2 = (c2+c1)%this->modulusVal;
        //std::cout << "c1: " << c1 << " - c2: " << c2 << std::endl;
    }

    return (uint32_t)(c1 << 16)+(uint16_t)c2;
}
