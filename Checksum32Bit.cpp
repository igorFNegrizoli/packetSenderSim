#include "Checksum32Bit.hpp"
#include <iostream>
#include <bitset>

bool Checksum32Bit::verifyChecksum(Packet* packet, uint32_t chk){
    return(this->doChecksum(packet) == chk);
}

uint32_t Checksum32Bit::doChecksum(Packet* packet){ 
    //falta isso aquiiii
    uint32_t checksum = 0;
    uint32_t *data32B = (uint32_t *)packet->getData();
    for(int i=0; i < packet->getLength()/4; i++){
        checksum += data32B[i];
        checksum += checksum < data32B[i];
        }
    return checksum;
}
