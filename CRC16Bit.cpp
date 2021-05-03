#include "CRC16Bit.hpp"
#include <iostream>
#include <cmath>

CRC16Bit::CRC16Bit(){
}
CRC16Bit::CRC16Bit(uint16_t len):packet(len){
    this->packetData[(len/2)-1] = this->doChecksum(this->packetData, 0x1021);
    this->checksum = this->packetData[(len/2)-1];
    this->poly = 0x1021;
}
CRC16Bit::CRC16Bit(uint16_t len, bool zeroOrOne):packet(len, zeroOrOne){
}

CRC16Bit::CRC16Bit(packet* basePacket):packet(basePacket){
}

bool CRC16Bit::verifyChecksum(){
    int i, j;
    uint16_t crc = 0;

    for (i = 0; i < (this->length/2); i++){
        crc ^= this->packetData[i];
        for (j = 0; j < 16; j++){
            if (crc & 1) crc ^= poly;
            crc >>= 1;
        }
    }
    return (crc==0x0000);
}

uint16_t CRC16Bit::doChecksum(uint16_t* packet, uint16_t poly){

    int i, j;
    uint16_t crc = 0;

    for (i = 0; i < ((this->length/2)-1); i++){
        crc ^= packet[i];
        for (j = 0; j < 16; j++){
            if (crc & 1) crc ^= poly;
            crc >>= 1;
        }
    }
    return crc;
}

uint16_t* CRC16Bit::getData(){
    uint16_t* packet_ = new uint16_t((this->length/2)-1);
    for(uint16_t i=0; i<this->length-1; i++) packet_[i] = this->packetData[i];
    return packet_;
}

CRC16Bit::~CRC16Bit(){
}