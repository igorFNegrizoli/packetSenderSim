#include "ErrorModel.hpp"
#include "Packet.hpp"
#include <iostream>

class BernoulliErrorModel: public ErrorModel {
private:
    double BER; //bit error rate     
public:        
    BernoulliErrorModel(double BER):ErrorModel() {
        this->BER = BER;
    }
     
    BernoulliErrorModel(double BER, RNG* rng):ErrorModel(rng) {
	   this->BER = BER;
    }
 
    double getBER() {
    	return this->BER;
    }

    void printAttributes(){
        std::cout << "Bernoulli Error Model: " << std::endl;
        std::cout << "BER: " << this->BER << std::endl;
    }

    uint16_t injectErrors(Packet* packet, bool forceError) {
        uint16_t numberOfErrors = 0;
        for(uint16_t i = 0; i<packet->getLength(); ++i){
            for(uint16_t j = 7; j<8; --j){
                if(getRNG()->trueFalseProb(this->getBER())==true){                    
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


