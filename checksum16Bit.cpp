#include "checksum16Bit.hpp"
#include <iostream>

checksum16Bit::checksum16Bit(){
}
checksum16Bit::checksum16Bit(uint16_t len):packet(len){
    //std::cout << "salve checksum16Bit" << std::endl;
    this->packetData[3] = 0x0000;
    this->packetData[3] = ~this->doChecksum(this->packetData, len);
    this->checksum = this->packetData[3];

}
checksum16Bit::checksum16Bit(uint16_t len, bool zeroOrOne):packet(len, zeroOrOne){
}

checksum16Bit::checksum16Bit(packet* basePacket):packet(basePacket){
}

bool checksum16Bit::verifyChecksum(){
    uint16_t chc = this->doChecksum(this->packetData, this->length);
    return(chc == 0xffff);
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

uint16_t* checksum16Bit::getData(){
    uint16_t* packet_ = new uint16_t((this->length/2)-1);
    for(uint16_t i=0; i<3; i++) packet_[i] = this->packetData[i];
    for(uint16_t i=4; i<this->length; i++) packet_[i-1] = this->packetData[i];
    return packet_;
}

checksum16Bit::~checksum16Bit(){
}