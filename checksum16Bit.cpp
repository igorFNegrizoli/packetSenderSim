#include "checksum16Bit.hpp"
#include "toolbox.hpp"
#include <iostream>

checksum16Bit::checksum16Bit(){
}
checksum16Bit::checksum16Bit(uint16_t len){
    this->length = len;
    uint16_t* packet_ = new uint16_t(len/2);
    for(uint16_t i=0; i<len/2; ++i){
        packet_[i] = randomIntInterval(0, 0xffff);
    }
    packet_[3] = 0x0000;
    packet_[2] = len;
    packet_[3] = ~this->doChecksum(packet_, len);
    this->checksum = packet_[3];
    this->packetData = packet_;
}
checksum16Bit::checksum16Bit(uint16_t len, bool zeroOrOne){
}


uint16_t checksum16Bit::doChecksum(uint16_t* packet, uint16_t length){
    uint32_t checksum_ = 0x00000000; //variavel de 32 bits para que seja possivel detectar o bit de carry
    uint16_t checksum = 0;
    for(int i=0; i < length/2; i++){
    checksum_ += packet[i];
        //if para detecção do bit de carry
        if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
        }
    }
    checksum = (uint16_t)checksum_;
    return checksum;
}

checksum16Bit::~checksum16Bit(){
}