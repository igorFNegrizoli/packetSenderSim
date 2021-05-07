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
    //packet->print('b');   
    uint32_t carry = 0x00000000; //variavel de 32 bits para que seja possivel detectar o bit de carry
    uint16_t checksum = 0;
    for(int i=0; i < packet->getLength()/2; i++){
        carry += packet->getData()[i];
	//cout << bitset<32>(carry) << endl;
        //if para detecção do bit de carry
        if ((0xffff0000 & carry) != 0x00000000){
            ++carry;
            carry &= 0x0000ffff;
	    //cout << "c"<<bitset<32>(carry) << endl;
        }
	
    }
    //cout << bitset<32>(carry) << endl;
    checksum = (uint16_t)carry;
    //cout << bitset<16>(checksum) << endl;
    return checksum;
}

