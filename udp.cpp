#include "udp.h"
#include "toolbox.h"
#include <iostream>
#include <cstdint>

using namespace std;

uint16_t doChecksum(uint16_t* packet, uint16_t length){
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

udpPacket::udpPacket(){
    this->length = 0;
    this->checksum = 0;
    this->packet = nullptr;
}

udpPacket::udpPacket(uint16_t len){
    this->length = len;
    uint16_t* packet_ = new uint16_t(len/2);
    for(uint16_t i=0; i<len/2; ++i){
        packet_[i] = randomIntInterval(0, 0xffff);
    }
    packet_[3] = 0x0000;
    packet_[2] = len;
    packet_[3] = ~doChecksum(packet_, len);
    this->checksum = packet_[3];
    this->packet = packet_;
}

uint16_t udpPacket::getLength(){
    return this->length;
}
uint16_t udpPacket::getChecksum(){
    return this->checksum;
}
void udpPacket::setLength(uint16_t a){
    this->length = a;
}
void udpPacket::setChecksum(){
    this->checksum = ~doChecksum(this->packet, this->length);//~ inverte os bits
}

bool udpPacket::verifyChecksum(){
    uint16_t chc = doChecksum(this->packet, this->length);
    if((chc + this->checksum) == 0xffff){
        return true;
    }else{
        return false;
    }
}

void udpPacket::printPacket(){
    cout << hex << "len: " << this->length << hex << " checksum: " << this->checksum << endl;
    for(int i=0;i<(this->length/2);++i){
        cout << hex << this->packet[i] << " ";
    }
}

udpPacket::~udpPacket(){
    delete [] packet;
}