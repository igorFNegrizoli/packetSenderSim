#include "ErrorModel.hpp"
#include "Packet.hpp"
#include <iostream>

class PeriodicBurstErrorModel: public ErrorModel {

private:
    uint16_t Nmin, Nmax; //Minumum and maximum burst length
    uint16_t Tmin, Tmax; //Minumum and maximum Period Threshold
public:        
    PeriodicBurstErrorModel(uint16_t N, uint16_t T, RNG* rng_):PeriodicBurstErrorModel(N,N,T,T,rng_){
    }
     
PeriodicBurstErrorModel(uint16_t Nmin, uint16_t Nmax, uint16_t Tmin, uint16_t Tmax, RNG* rng_):ErrorModel(rng_) {
	this->Nmin = Nmin;
    this->Nmax = Nmax+1;
    this->Tmin = Tmin;
    this->Tmax = Tmax+1;
    this->rng = rng_;
}

uint16_t injectErrors(Packet* packet, bool forceError) {
    // Usar Tmin > 0 e Nmin > 0     Para um threshold(T) ou burstLength(N) de tamanho fixo, usar Tmin = Tmax e Nmin = Nmax
    // Use Tmin > 0 e Nmin > 0      For a fixed length threshold(T) or burstLength(N), use Tmin = Tmax and Nmin = Nmax
    uint16_t i, pos;
    uint16_t threshold, eLength;
    uint16_t numberOfErrors;

    threshold = this->rng->next(Tmin, Tmax);
    eLength = this->rng->next(Nmin, Nmax);

    for(i=0; i<packet->getLength()/2; ++i){
        for(pos=15; pos<16; --pos){        
            if(eLength > 0){
                packet->injectErrorInChunk(i*16+pos);
                ++numberOfErrors;
                --eLength;
            }
            --threshold;
            if(threshold == 0){
                threshold = this->rng->next(Tmin, Tmax);
                eLength = this->rng->next(Nmin, Nmax);    
            }
        }
    }

	if (numberOfErrors == 0 && forceError)
        numberOfErrors+=packet->injectErrorInChunk(getRNG()->next(packet->getLength()));
        return numberOfErrors;
    }

};
