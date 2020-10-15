#include "toolbox.h"
#include <iostream>
using namespace std;
/*
struct UdpPacket{
    uint16_t srcPort, dstPort, length, checksum;
    uint16_t* data;
}typedef UdpPacket;

struct bit16ErrorVec{
    uint16_t nOfErrors = 0;
    bool errorVec[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}typedef bit16ErrorVec;


uint16_t doChecksum(uint16_t srcPort_ = 0, uint16_t dstPort_ = 0, uint16_t length_ = 8, uint16_t* data_ = nullptr){

    uint32_t checksum_ = 0x00000000;
    uint32_t checksum = 0;
    checksum_ += srcPort_;
    checksum_ += dstPort_;
    if ((0xffff0000 & checksum_) != 0x00000000){
        ++checksum_;
        checksum_ = checksum_ & 0x0000ffff;
    }
    checksum_ += length_;
    if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
    }
    if (data_ == nullptr) {
        checksum = checksum_;
        return checksum;
    }
    for(int i=0; i < (length_ - 8)/2; i++){
        checksum_ += data_[i];
        if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
        }
    }
    checksum = checksum_;
    return checksum;
}


bool receiveUdpPacket(UdpPacket* packet=nullptr){
    uint16_t chc;
    chc = doChecksum(packet->srcPort, packet->dstPort, packet->length, packet->data);
    if(chc + packet->checksum != 0xffff){
        return false;
    }
    return true;
}


UdpPacket* injectErrorInPacket(UdpPacket* pkg, uint16_t errorQuantity = 1){
    16bitErrorVec packageErrors[pkg->length];
    for(uint16_t i=0; i<errorQuantity; ++i){
        do{
            uint16_t position = randomnum(pkg->length);
        }while(packageErrors[position].nOfErrors >= 16);

    }
}

UdpPacket* buildUdpPacket(uint16_t srcPort_ = 0, uint16_t dstPort_ = 0, uint16_t length_ = 8, uint16_t* data_ = nullptr){
    UdpPacket* pkg = new UdpPacket;
    pkg->srcPort = srcPort_;
    pkg->dstPort = dstPort_;
    pkg->length = length_;
    pkg->checksum = ~doChecksum(srcPort_, dstPort_, length_, data_);
    pkg->data = data_;
    return pkg;
}*/
struct bit16ErrorVec{
    uint16_t nOfErrors = 0;
    bool errorVec[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}typedef bit16ErrorVec;

uint16_t doChecksum(uint16_t* packet = nullptr){

    uint32_t checksum_ = 0x00000000;
    uint16_t checksum = 0;
    for(int i=0; i < packet[2]/2; i++){
        checksum_ += packet[i];
        if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
        }
    }
    checksum = checksum_;
    return checksum;
}

bool receiveUdpPacket(uint16_t* packet=nullptr){
    uint16_t chc;
    chc = doChecksum(packet);
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

uint16_t* injectErrorInPacket(uint16_t* packet, uint32_t errorQuantity = 1){
    bit16ErrorVec packageErrors[packet[2]/2];
    if(errorQuantity > (packet[2])*8) return nullptr;
    uint16_t position, microPosition;
    uint16_t limit = 30;
    for(uint16_t i=0; i<errorQuantity; ++i){
        /*
        Esse for (abaixo) serve pra sortear um chunk no pacote para sofrer um erro 
        Sim, parece mais complicado do que deveria ser, mas isso é pra garantir que apos tentar escolher uma
        cadeia de 16 bits que tem menos de 16 alteraçoes um determinado numero de vezes e falhar, o algoritmo escolhe a primeira
        palavra de 16 bits que ainda tem bits que nao foram alterados
        */
        for(uint16_t j = 0; j<=limit; ++j){
            position = randomnum(packet[2]/2);
            if(packageErrors[position].nOfErrors < 16) break;
            if(j == limit){
                for(uint16_t k = 0; k<(packet[2]/2); ++k){
                    if(packageErrors[k].nOfErrors < 16){
                        position = k;
                        break;
                    }
                }
            }
        }
        //Fiz a mesma coisa que descrevi no comentario anterior mas para escolher qual bit sera alterado em vez da palavra
        for(uint16_t j = 0; j<=limit; ++j){
            microPosition = randomnum(16);
            if(packageErrors[position].errorVec[microPosition] == 0) break;
            if(j == limit){
                for(uint16_t k = 0; k<16; ++k){
                    if(packageErrors[position].errorVec[microPosition] == 0){
                        microPosition = k;
                        break;
                    }
                }
            }
        }
        //cout << "-------" << endl << position << " : " << microPosition << endl <<"-------" << endl;
        //Agora que as posiçoes foram escolhidas, chama-se a funçao que altera o bit
        //packet[position] = injectErrorInChunk(packet[position], microPosition);

    }
    return 0;
}