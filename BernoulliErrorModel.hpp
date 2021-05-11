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

    uint16_t injectErrors(Packet* packet, bool forceError) {
    	uint16_t numberOfErrors = 0;
    	for(uint16_t i = 0; i<packet->getLength()/2; ++i){
             for(uint16_t j = 15; j<16; --j){
                if(getRNG()->trueFalseProb(this->getBER())==true){                    
                    numberOfErrors+=packet->injectErrorInChunk(i*16+j);
                }
             }
        }  
	if (numberOfErrors == 0 && forceError)
	    numberOfErrors+=packet->injectErrorInChunk(getRNG()->next(packet->getLength())); 
    	
        return numberOfErrors;
    }

};


