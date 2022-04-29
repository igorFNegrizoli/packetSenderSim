#include "Checksum16Bit.hpp"
//#include "Packet.hpp"
#include <iostream>
#include <bitset>
using namespace std;

bool Checksum16Bit::verifyChecksum(Packet* packet, uint16_t chk){
    //packet.print('b');
    //cout<<"verify:"<<chk<<endl;
    uint16_t _chk = this->doChecksum(packet);
    //cout<<"_chk"<<endl;
    return(_chk == chk);
}

uint16_t Checksum16Bit::doChecksum(Packet* packet){ 
    uint16_t checksum = 0;
    for(int i=0; i < packet->getLength()/2; i++){
        checksum += packet->getData()[i];
        checksum += checksum < packet->getData()[i];
        }
    return checksum;
}
