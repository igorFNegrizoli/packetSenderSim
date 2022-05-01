#include "Checksum16Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>

bool Checksum16Bit::verifyChecksum(Packet* packet, uint16_t chk){
    return(this->doChecksum(packet); == chk);
}

uint16_t Checksum16Bit::doChecksum(Packet* packet){ 
    uint16_t checksum = 0;
    for(int i=0; i < packet->getLength()/2; i++){
        checksum += packet->getData()[i];
        checksum += checksum < packet->getData()[i];
        }
    return checksum;
}
