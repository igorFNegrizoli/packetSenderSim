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

CRC16Bit::~CRC16Bit(){
}