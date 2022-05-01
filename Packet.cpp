#include "Packet.hpp"
#include <iostream>
#include <cmath>
#include <cstdint>
#include <bitset>
#include <stdlib.h>
#include <time.h>
#include <random>

Packet::Packet(){
    Packet(new RNG());
}

Packet::Packet(RNG* rng_){
    this->length = 0;
    this->data = nullptr;
    this->rng = rng_;
}

Packet::Packet(uint16_t len){
    Packet(len, new RNG());  
}

Packet::Packet(uint16_t len, RNG* rng_){
    this->length = len;
    this->rng = rng_;
    this->data = (uint8_t*)malloc(len);
    for(uint16_t i=0; i<len; ++i){
        this->data[i] = this->rng->next(0x00, 0xff);
    }
}

Packet* Packet::clone() {
    //RNG *rng = new RNG(this->rng->getSeed());
    Packet *p = new Packet(this->length, this->rng); //essa chamada faz as atribuiçoes aleatorias desnecessariamente, tem que arrumar
    for (int i=0; i<p->length; ++i) {
	p->data[i] = this->data[i];
    }
    return p;
}

/*
Esse construtor tem o proposito de auxiliar a visualização dos efeitos dos modelos
de erro apenas. Constroi um pacote com apenas zeros ou apenas uns

This constructor's purpose is to hel visualize the effects of the error models only.
Constructs a packet with only zeroes or only ones.
*/
Packet::Packet(uint16_t len, bool zeroOrOne){
    this->length = len;
    this->data = (uint8_t*)malloc(len);;
    if (zeroOrOne){
        for(uint16_t i=0; i<len; ++i){
            data[i] = 0xff;
        }    
    }else{
        for(uint16_t i=0; i<len; ++i){
            data[i] = 0x00;
        }
    }
}

uint8_t* Packet::getData(){
    return this->data;
}

uint16_t Packet::getLength(){
    return this->length;
}

void Packet::print(char mode){
    std::cout << "pkg len: " << this->length <<" bytes"<< std::endl;
    switch(mode){
        case 'h':
            for(int i=0;i<(this->length);++i) std::cout << std::hex << (int)this->data[i] << " ";
                break;
        case 'b':
            for(int i=0;i<(this->length);++i) std::cout << std::bitset<8>((int)this->data[i]) << " ";
            break;
    }
    std::cout << std::endl;
}

//Flippa o bit na posição "bitpos" do packet
//Flips the "bitpos" bit of the data packet
uint16_t Packet::injectErrorInChunk(uint64_t bit_pos){    
    uint64_t sizeOfPacketInBits = this->length * 8;
    if(bit_pos >= sizeOfPacketInBits) {
	std::cout << "WARNING: injectErrorInChunk -> bit_pos: "<<bit_pos<< "> size of packet: "<<sizeOfPacketInBits << std::endl;
	return 0;
    } 
    //determinar o indice no array de data
    int word = bit_pos / 8.0;
    int pos  = bit_pos % 8;
    this->data[word] ^= uint8_t(pow(2,pos));
    return 1;
}

//Flippa os bits [bit_start, bit_end] do packet
//Flips the [bit_start, bit_end] bits of the data packet
uint16_t Packet::injectErrorInChunk(uint64_t bit_start, uint64_t bit_end){
    uint64_t sizeOfPacketInBits = this->length * 8;
    if(bit_start > bit_end) {
	std::cout << "WARNING: injectErrorInChunk -> bit_start: "<<bit_start<<" > bit_end: "<<bit_end << std::endl;
	return 0;
    } 
    if(bit_start >= sizeOfPacketInBits) {
	std::cout << "WARNING: injectErrorInChunk -> bit_start: "<<bit_start<<" > size of packet: " <<sizeOfPacketInBits << std::endl;
	return 0;
    } 
    if(bit_end >= sizeOfPacketInBits) {
	bit_end = sizeOfPacketInBits-1;
	std::cout << "WARNING: injectErrorInChunk -> bit_end: "<<bit_end<<" set with size of packet: " <<sizeOfPacketInBits << std::endl;
    }
    uint16_t numberOfErrors = 0; 
    for (uint64_t pos = bit_start; pos<=bit_end; pos++) {
	numberOfErrors+=injectErrorInChunk(pos);
    }
    return numberOfErrors;
}


Packet::~Packet(){
    delete [] data;
}
