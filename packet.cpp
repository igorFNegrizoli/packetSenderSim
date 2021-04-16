#include "packet.hpp"
//#include "toolbox.hpp"
#include <iostream>
#include <cmath>
#include <cstdint>
#include <bitset>
#include <stdlib.h>
#include <time.h>
#include <random>
//#include <iomanip>

packet::packet(){
    this->length = 0;
    this->checksum = 0;
    this->packetData = nullptr;
}

packet::packet(uint16_t len){
    
    this->length = len;
    uint16_t* packet_ = new uint16_t(len/2);
    for(uint16_t i=0; i<len/2; ++i){
        packet_[i] = this->randomIntInterval(0, 0xffff);
    }
    packet_[3] = 0x0000;
    packet_[2] = len;
    packet_[3] = ~this->doChecksum(packet_, len);
    this->checksum = packet_[3];
    this->packetData = packet_;
    
}

/*
Esse construtor tem o proposito de auxiliar a visualização dos efeitos dos modelos
de erro apenas. Constroi um pacote com apenas zeros ou apenas uns

This constructor's purpose is to hel visualize the effects of the error models only.
Constructs a packet with only zeroes or only ones.
*/
packet::packet(uint16_t len, bool zeroOrOne){
    this->length = len;
    uint16_t* packet_ = new uint16_t(len/2);
    if (zeroOrOne){
        for(uint16_t i=0; i<len/2; ++i){
            packet_[i] = 0xffff;
        }    
    }else{
        for(uint16_t i=0; i<len/2; ++i){
            packet_[i] = 0;
        }
    }
    
    this->checksum = packet_[3];
    this->packetData = packet_;
}

uint16_t packet::getLength(){
    return this->length;
}
uint16_t packet::getChecksum(){
    return this->checksum;
}
void packet::setLength(uint16_t a){
    this->length = a;
}
void packet::setChecksum(){
    this->checksum = ~this->doChecksum(this->packetData, this->length);//~ inverte os bits
}

bool packet::verifyChecksum(){
    uint16_t chc = this->doChecksum(this->packetData, this->length);
    return(chc == 0xffff);
}

void packet::printPacket(char mode){
    std::cout << std::hex << "len: " << this->length << std::hex << " checksum: " << this->checksum << std::endl;
    switch(mode){
        case 'h':
            for(int i=0;i<(this->length/2);++i) std::cout << std::hex << this->packetData[i] << " ";
                break;
        case 'b':
            for(int i=0;i<(this->length/2);++i) std::cout << std::bitset<16>(this->packetData[i]) << " ";
            break;
    }
    std::cout << std::endl;
}

//Flippa o bit na posição "microPosition" da palavra de posicao "pos" do packet
//Flips the "microPosition" bit of the word in "pos" of packet
void packet::injectErrorInChunk(uint16_t pos, uint16_t microPosition){
    if(microPosition >= 16) std::cout << "warning: pos >= in injectErrorInChunk" << std::endl;
    uint16_t auxiliary = uint16_t(pow(2,microPosition));
    this->packetData[pos] = (this->packetData[pos] ^ auxiliary);
}

//As seguintes implementações foram baseadas na descrição dada nesse paper www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf
//The following implementations are based in the description given in this paper www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf

//Modelo de bernoulli descrito no artigo citado
//Bernoulli model mentioned in the said article
void packet::bernoulliModel(double BER){
    bool wasPacketModified = false;
    while(!wasPacketModified){
    //This loop's purpose is to ensure the packet has at least one error
        for(uint16_t i = 0; i<this->length/2; ++i){
            for(uint16_t j = 0; j<16; ++j){
                if(this->trueFalseProb(BER) == true){
                    wasPacketModified = true;
                    this->injectErrorInChunk(i, j);
                }
            }
        }    
    }
}

//Funcionamento do algoritmo está descrito no item 4 do paper citado no comentario da implementação do modelo de Gilbert-Elliot
//How the algorithm works is described at the previous mentioned article
void packet::burstErrorPeriodicModel(int16_t Tmin, uint16_t Tmax, uint16_t Nmin, uint16_t Nmax){
    // Usar Tmin > 0 e Nmin > 0     Para um threshold(T) ou burstLength(N) de tamanho fixo, usar Tmin = Tmax e Nmin = Nmax
    // Use Tmin > 0 e Nmin > 0      For a fixed length threshold(T) or burstLength(N), use Tmin = Tmax and Nmin = Nmax
    uint16_t i, pos; //T -> threshold, N -> burst length
    uint16_t threshold, eLength;
    uint16_t accumulator = 0;

    threshold = this->randomIntInterval(Tmin, Tmax);
    eLength = this->randomIntInterval(Nmin, Nmax);
    accumulator += threshold;

    for(i=0; i<this->length/2; ++i){
        for(pos=15; pos<16; --pos){
            if(eLength > 0){
                this->injectErrorInChunk(i, pos);
                --eLength;
            }
            --threshold;

            if(threshold == 0){
                threshold = this->randomIntInterval(Tmin, Tmax);
                eLength = this->randomIntInterval(Nmin, Nmax);
                accumulator += threshold;
            }
        }
    }
}

//Funcionamento do algoritmo está descrito no item 3 do paper citado anteriormente
//How the algorithm works is described at the previous mentioned article
void packet::gilbertModel(uint16_t burst, double plRate){
    //q e p representam a probabilidade de passar para o estado bom e para o estado ruim respectivamente
    //q e p represents the probability of going to qhe good state and the bad state respectively

    //para calcular a probabilidade de uma rajada de tamanho k acontecer usa-se pk=q(1-q)^(k-1)
    //to calculate the probability of a burst error occurring pk=q(1-q)^(k-1)

    //calculos de p e q realizados conforme Yu, X. & Modestino, J (2008)
    //p and q calculation according to Yu, X. & Modestino, J, (2008)

    double q = double(1)/burst;
    double p = double(q*plRate)/(1-plRate);
    //std::cout << std::fixed << std::setprecision(7) << "p: " << p << " q: " << q << std::endl;

    bool state = true;//good state

    bool wasPacketModified = false;
    while(!wasPacketModified){
    //This loop's purpose is to ensure the packet has at least one error
        for(uint16_t i=0; i<this->length/2; ++i){
            for(uint16_t pos=15; pos<16; --pos){
                if(state == true){
                    if(this->trueFalseProb(1-p)){
                        state = true;
                    }else{
                        this->injectErrorInChunk(i, pos);
                        wasPacketModified = true;
                        //std::cout << i << " : " << pos << std::endl;
                        state = false;
                    }
                }else{
                    if(this->trueFalseProb(1-q)){
                        this->injectErrorInChunk(i, pos);
                        wasPacketModified = true;
                        //std::cout << i << " : " << pos << std::endl;
                        state = false;
                    }else{
                        state = true;
                    }
                }
            }
        }
    }
}

uint16_t packet::randomnum(uint16_t max){
    srand (clock());
    return (rand() % max);
}
double packet::randZeroToOne(){
    srand (clock());
    return rand() / (RAND_MAX + 1.);
}
bool packet::trueFalseProb(double limit){
    limit = limit - randZeroToOne();
    if(limit > 0) return true;
    return false;
}
uint16_t packet::randomIntInterval(uint16_t a, uint16_t b){
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(a, b); // define the range
    return distr(gen);
}


packet::~packet(){
    delete [] packetData;
}