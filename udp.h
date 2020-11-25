#include <iostream>
#include <math.h>

using namespace std;

//Implementação mais concisa, legivel e rápida que a a versão anterior de 100+ linhas

uint16_t doChecksum(uint16_t* packet = nullptr, uint16_t len = 0){
    if(len==0) len = packet[2];
    uint32_t checksum_ = 0x00000000; //variavel de 32 bits para que seja possivel detectar o bit de carry
    uint16_t checksum = 0;
    for(int i=0; i < len/2; i++){
        checksum_ += packet[i];
        //if para detecção do bit de carry
        if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
        }
    }
    checksum = checksum_;
    return checksum;
}

bool receiveUdpPacket(uint16_t* packet=nullptr, uint16_t len = 0){
    if(len==0) len = packet[2];
    uint16_t chc;
    chc = doChecksum(packet, len);
    if(chc + packet[3] != 0xffff){
        return false;
    }
    return true;
}


uint16_t* buildUdpPacket(uint16_t srcPort_ = 0, uint16_t dstPort_ = 0, uint16_t length_ = 8, uint16_t* data_ = nullptr){
    uint16_t* packet = new uint16_t(length_/2);
    packet[0] = srcPort_;
    packet[1] = dstPort_;
    packet[2] = length_;
    packet[3] = 0;
    for(uint16_t i=4; i<length_/2; ++i){
        packet[i] = data_[i-4];
    }
    packet[3] = ~doChecksum(packet);
    return packet;
}