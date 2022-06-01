#include "ErrorModel.hpp"
#include "Packet.hpp"
#include <iostream>
#include <iomanip>//setprecision setfixed
#include <bitset>

#define BAD_STATE true
#define GOOD_STATE false

class GilbertErrorModel: public ErrorModel {

private:
    int burst;
    double plRate;
     
public:        
    GilbertErrorModel(int burst_, double plRate_):ErrorModel() {
    	this->burst = burst_;
	    this->plRate = plRate_;
    }
     
    GilbertErrorModel(int burst_, double plRate_, RNG* rng_):ErrorModel(rng) {
	    this->burst = burst_;
	    this->plRate = plRate_;
	    this->rng = rng_;
    }

    void printAttributes(){
        std::cout << "Gilbert Error Model: " << std::endl;
        std::cout << "Average Burst Length: " << this->burst << std::endl;
        std::cout << "Burst Occurrence Rate: " << this->plRate << std::endl;
    }
     
    int getBurst() {
	    return this->burst;
    }
     
    double getPlRate() {
	    return this->plRate;
    }

    double getP() {
	    return double(getQ()*plRate)/(1-plRate);
    }

    double getQ() {
	    return double(1)/burst;
    }

    uint16_t injectErrors(Packet* packet, bool forceError) {
	//packet->print('b');
    	uint16_t numberOfErrors = 0;
    	//q e p representam a probabilidade de passar para o estado bom e para o estado ruim respectivamente
    	//q e p represents the probability of going to qhe good state and the bad state respectively

    	//para calcular a probabilidade de uma rajada de tamanho k acontecer usa-se pk=q(1-q)^(k-1)
    	//to calculate the probability of a burst error occurring pk=q(1-q)^(k-1)

    	//calculos de p e q realizados conforme Yu, X. & Modestino, J (2008)
   	 //p and q calculation according to Yu, X. & Modestino, J, (2008)

    	double q = double(1)/burst;
    	double p = double(q*plRate)/(1-plRate);
    	//std::cout << std::fixed << std::setprecision(7) << "p: " << p << " q: " << q << std::endl;

    	bool state = GOOD_STATE;//good state

    	bool wasPacketModified = false;
    	for(int i=0; i<packet->getLength(); ++i){
            //std::cout << "\nb"<<std::bitset<16>(packet->getData()[i]) <<std::endl;
            for(uint16_t j=7; j<8; --j){
                if(state == GOOD_STATE){
		    if(rng->trueFalseProb(1-p) == false){
                        numberOfErrors+=packet->injectErrorInChunk(i*8+j);
                        wasPacketModified = true;
                        //std::cout << i << " : " << j << std::endl;
                        state = BAD_STATE;
                    }                    
                }else{ //it is in BAD_STATE
                    if(rng->trueFalseProb(1-q)){
                        numberOfErrors+=packet->injectErrorInChunk(i*8+j);
                        wasPacketModified = true;
                        //std::cout << i << " : " << j << std::endl;
                        state = BAD_STATE;
                    } else {
 			state = GOOD_STATE;
		    }
                }
            }
           // std::cout << "a"<<std::bitset<16>(packet->getData()[i]) <<std::endl;
        }

    	if (numberOfErrors == 0 && forceError)
    	    numberOfErrors+=packet->injectErrorInChunk(getRNG()->next(packet->getLength())); 

        packet->updateBuffers();
        return numberOfErrors;
    }

};


