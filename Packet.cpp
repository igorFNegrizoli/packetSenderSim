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
    this->data = new uint16_t[len/2];//=len/2 words 16 bits
    for(uint16_t i=0; i<len/2; ++i){
        this->data[i] = this->rng->next(0x0000, 0xffff);
    }
}

Packet* Packet::clone() {
    //RNG *rng = new RNG(this->rng->getSeed());
    Packet *p = new Packet(this->length, this->rng);
    for (int i=0; i<p->length/2; ++i) {
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
    this->data = new uint16_t(len/2);
    if (zeroOrOne){
        for(uint16_t i=0; i<len/2; ++i){
            data[i] = 0xffff;
        }    
    }else{
        for(uint16_t i=0; i<len/2; ++i){
            data[i] = 0;
        }
    }
}

uint16_t* Packet::getData(){
    return this->data;
}

uint16_t Packet::getLength(){
    return this->length;
}

void Packet::print(char mode){
    std::cout << "pkg len: " << this->length <<" bytes"<< std::endl;
    switch(mode){
        case 'h':
            for(int i=0;i<(this->length/2);++i) std::cout << std::hex << this->data[i] << " ";
                break;
        case 'b':
            for(int i=0;i<(this->length/2);++i) std::cout << std::bitset<16>(this->data[i]) << " ";
            break;
    }
    std::cout << std::endl;
}

//Flippa o bit na posição "bitpos" do packet
//Flips the "bitpos" bit of the data packet
uint16_t Packet::injectErrorInChunk(uint16_t bit_pos){    
    uint16_t sizeOfPacketInBits = this->length * 8;
    if(bit_pos >= sizeOfPacketInBits) {
	std::cout << "WARNING: injectErrorInChunk -> bit_pos: "<<bit_pos<< "> size of packet: "<<sizeOfPacketInBits << std::endl;
	return 0;
    } 
    //determinar o indice no array de data
    int word = bit_pos / 16.0;
    int pos  = bit_pos % 16;
    //std::cout << bit_pos << " "<<word<<", "<<pos<<std::endl;
    uint16_t auxiliary = uint16_t(pow(2,pos));
    //std::cout << auxiliary<<std::endl;
    //std::cout << std::bitset<16>(this->data[word]) <<std::endl;	
    this->data[word] ^= auxiliary;
    //std::cout << std::bitset<16>(this->data[word]) <<std::endl;
    return 1;
}

//Flippa os bits [bit_start, bit_end] do packet
//Flips the [bit_start, bit_end] bits of the data packet
uint16_t Packet::injectErrorInChunk(uint16_t bit_start, uint16_t bit_end){
    uint16_t sizeOfPacketInBits = this->length * 8;
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
    for (uint16_t pos = bit_start; pos<=bit_end; pos++) {
	numberOfErrors+=injectErrorInChunk(pos);
    }
    return numberOfErrors;
}


Packet::~Packet(){
    delete [] data;
}
