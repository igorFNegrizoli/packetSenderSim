#include "ErrorModel.hpp"
#include "Packet.hpp"
#include <iostream>
#include <bitset>
#include <iomanip>//setprecision setfixed

#define FIXED_FLOAT(p, x) std::fixed << std::setprecision(p) <<(x)

class SparseBurstsErrorModel: public ErrorModel {
private:
    double BOR; //burst occurrence rate
    uint16_t Fmin;
    uint16_t Fmax;
public:        
    SparseBurstsErrorModel(double BOR, uint16_t F_):ErrorModel() {
        this->BOR = BOR;
        this->Fmin = F_;
        this->Fmax = F_;
    }

    SparseBurstsErrorModel(double BOR, uint16_t Fmin_, uint16_t Fmax_):ErrorModel() {
        this->BOR = BOR;
        this->Fmin = Fmin_;
        this->Fmax = Fmax_;
    }
     
    SparseBurstsErrorModel(double BOR, uint16_t F_,  RNG* rng):ErrorModel(rng) {
        this->BOR = BOR;
        this->Fmin = F_;
        this->Fmax = F_;
    }

    SparseBurstsErrorModel(double BOR, uint16_t Fmin_, uint16_t Fmax_,  RNG* rng):ErrorModel(rng) {
        this->BOR = BOR;
        this->Fmin = Fmin_;
        this->Fmax = Fmax_;
    }

    void printAttributes(){
        std::cout << "Sparse Bursts Error Model: " << std::endl;
        std::cout << "BOR: " << FIXED_FLOAT(5, this->BOR) << std::endl;
        std::cout << "F: [" << this->Fmin << ", " << this->Fmax << "]" << std::endl;
    }

    uint16_t injectErrors(Packet* packet, bool forceError) {
        uint16_t numberOfErrors = 0;

        uint16_t i = 0, j = 7;
        uint16_t eLength = 0;
        bool lastError = false;

        for(uint16_t i=0; i<packet->getLength(); ++i){
            for(uint16_t j=7; j<8; --j){
                if(eLength > 0){
                    numberOfErrors+=packet->injectErrorInChunk(i*8+j);
                    eLength--;
                    if (eLength == 0)
                        lastError = true;
                }else if(lastError){
                    lastError = false;
                }else if(getRNG()->trueFalseProb(this->BOR)){
                    eLength = this->rng->next(Fmin, Fmax)-1;
                    numberOfErrors+=packet->injectErrorInChunk(i*8+j);
                }
            }
        }

        if (numberOfErrors == 0 && forceError)
            numberOfErrors+=packet->injectErrorInChunk(getRNG()->next(packet->getLength()));

        packet->updateBuffers();

        return numberOfErrors;
    }

};