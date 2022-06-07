#include "Fletcher32Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>

/*
(0,0,65535) for ones complement fletcher
(0,0,65536) for twos complement fletcher
(1,0,65521) for adler
*/
Fletcher32Bit::Fletcher32Bit(bool is1sComp){
    this->initialC1 = 0;
    this->initialC2 = 0;

    if(is1sComp){
        this->modulusVal = 65535;
    }else{
        this->modulusVal = 65536;
    }
}

uint32_t Fletcher32Bit::doChecksum(Packet* packet){ 
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
