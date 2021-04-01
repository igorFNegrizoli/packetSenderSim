#include "udp.hpp"
#include "toolbox.hpp"
#include <iostream>
#include <cstdint>
#include <bitset>

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

/*
Esse construtor tem o proposito de auxiliar a visualização dos efeitos dos modelos
de erro apenas. Constroi um pacote com apenas zeros ou apenas uns

This constructor's purpose is to hel visualize the effects of the error models only.
Constructs a packet with only zeroes or only ones.
*/
udpPacket::udpPacket(uint16_t len, bool zeroOrOne){
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
    return(chc == 0xffff);
}

void udpPacket::printPacket(char mode){
    cout << hex << "len: " << this->length << hex << " checksum: " << this->checksum << endl;
    switch(mode){
        case 'h':
            for(int i=0;i<(this->length/2);++i) cout << hex << this->packet[i] << " ";
                break;
        case 'b':
            for(int i=0;i<(this->length/2);++i) cout << bitset<16>(this->packet[i]) << " ";
            break;
    }
    cout << endl;
}

//Flippa o bit na posição "microPosition" da palavra de posicao "pos" do packet
//Flips the "microPosition" bit of the word in "pos" of packet
void udpPacket::injectErrorInChunk(uint16_t pos, uint16_t microPosition){
    if(microPosition >= 16) cout << "warning: pos >= in injectErrorInChunk" << endl;
    uint16_t auxiliary = pow(2,microPosition);
    this->packet[pos] = (this->packet[pos] ^ auxiliary);
}

//As seguintes implementações foram baseadas na descrição dada nesse paper www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf
//The following implementations are based in the description given in this paper www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf

//Modelo de bernoulli descrito no artigo citado
//Bernoulli model mentioned in the said article
void udpPacket::bernoulliModel(double BER){
    bool wasPacketModified = false;
    while(!wasPacketModified){
    //This loop's purpose is to ensure the packet has at least one error
        for(uint16_t i = 0; i<this->length/2; ++i){
            for(uint16_t j = 0; j<16; ++j){
                if(trueFalseProb(BER) == true){
                    wasPacketModified = true;
                    this->injectErrorInChunk(i, j);
                }
            }
        }    
    }
}

//Funcionamento do algoritmo está descrito no item 4 do paper citado no comentario da implementação do modelo de Gilbert-Elliot
//How the algorithm works is described at the previous mentioned article
void udpPacket::burstErrorPeriodicModel(int16_t Tmin, uint16_t Tmax, uint16_t Nmin, uint16_t Nmax){
    // Usar Tmin > 0 e Nmin > 0     Para um threshold(T) ou burstLength(N) de tamanho fixo, usar Tmin = Tmax e Nmin = Nmax
    // Use Tmin > 0 e Nmin > 0      For a fixed length threshold(T) or burstLength(N), use Tmin = Tmax and Nmin = Nmax
    uint16_t i, pos; //T -> threshold, N -> burst length
    uint16_t threshold, eLength;
    uint16_t accumulator = 0;

    threshold = randomIntInterval(Tmin, Tmax);
    eLength = randomIntInterval(Nmin, Nmax);
    accumulator += threshold;

    for(i=0; i<this->length/2; ++i){
        for(pos=15; pos<16; --pos){
            if(eLength > 0){
                this->injectErrorInChunk(i, pos);
                --eLength;
            }
            --threshold;

            if(threshold == 0){
                threshold = randomIntInterval(Tmin, Tmax);
                eLength = randomIntInterval(Nmin, Nmax);
                accumulator += threshold;
            }
        }
    }
}

//Funcionamento do algoritmo está descrito no item 3 do paper citado anteriormente
//How the algorithm works is described at the previous mentioned article
void udpPacket::gilbertModel(uint16_t burst, double plRate){
    //q e p representam a probabilidade de passar para o estado bom e para o estado ruim respectivamente
    //q e p represents the probability of going to qhe good state and the bad state respectively

    //para calcular a probabilidade de uma rajada de tamanho k acontecer usa-se pk=q(1-q)^(k-1)
    //to calculate the probability of a burst error occurring pk=q(1-q)^(k-1)

    //calculos de p e q realizados conforme Yu, X. & Modestino, J (2008)
    //p and q calculation according to Yu, X. & Modestino, J, (2008)

    double q = (1/burst);
    double p = (q*plRate)/(1-plRate);

    bool state = true;//good state

    bool wasPacketModified = false;
    while(!wasPacketModified){
    //This loop's purpose is to ensure the packet has at least one error
        for(uint16_t i=0; i<this->length/2; ++i){
            for(uint16_t pos=15; pos<16; --pos){
                if(state == true){
                    if(trueFalseProb(1-p)){
                        state = true;
                    }else{
                        this->injectErrorInChunk(i, pos);
                        wasPacketModified = true;
                        //cout << i << " : " << pos << endl;
                        state = false;
                    }
                }else{
                    if(trueFalseProb(1-q)){
                        this->injectErrorInChunk(i, pos);
                        wasPacketModified = true;
                        //cout << i << " : " << pos << endl;
                        state = false;
                    }else{
                        state = true;
                    }
                }
            }
        }
    }
}

udpPacket::~udpPacket(){
    delete [] packet;
}