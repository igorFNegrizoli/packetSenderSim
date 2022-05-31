#include "FletcherAdler16Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>

/*
(0,0,255) for ones complement fletcher
(0,0,256) for twos complement fletcher
(1,0,251) for adler
*/
FletcherAdler16Bit::FletcherAdler16Bit(uint16_t initialC1_, uint16_t initialC2_, uint16_t modulusVal_){
    this->initialC1 = initialC1_;
    this->initialC2 = initialC2_;
    this->modulusVal = modulusVal_;
}

uint16_t FletcherAdler16Bit::doChecksum(Packet* packet){ 
    uint8_t* data = (uint8_t*)packet->getData();
    uint16_t c1 = this->initialC1;
    uint16_t c2 = this->initialC2;

    for(int i=0; i < packet->getLength(); i++){
        c1 = (c1+data[i])%this->modulusVal;
        c2 = (c2+c1)%this->modulusVal;
        //std::cout << "c1: " << c1 << " - c2: " << c2 << std::endl;
    }

    return (uint16_t)(c1 << 8)+(uint8_t)c2;
}
