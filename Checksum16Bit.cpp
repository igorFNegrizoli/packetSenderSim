#include "Checksum16Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>

bool Checksum16Bit::verifyChecksum(Packet* packet, uint16_t chk){
    return(this->doChecksum(packet) == chk);
}

/*
little endian is not treated here because the "data loss" doesnt matter
since the data consists of random values, it just makes debugging difficult :(
There will be problems with we try to track down error patterns, but since 
its not made here, we can ignore this issue for now
*/
uint16_t Checksum16Bit::doChecksum(Packet* packet){ 
    uint16_t checksum = 0;
    uint16_t* data16B = (uint16_t*)packet->getData();
    for(int i=0; i < packet->getLength()/2; i++){
        checksum += data16B[i];
        checksum += checksum < data16B[i];
        }
    return checksum;
}
