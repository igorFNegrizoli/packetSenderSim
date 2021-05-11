#include "RNG.hpp"
#include <cstdint>
#include <cstdlib>
#include <time.h> 
#include <iostream>

RNG::RNG() {
   this->seed = (uint16_t)clock();
   srand (seed);
}

RNG::RNG(uint16_t seed) {
   this->seed = seed;
   srand (seed);
}

uint16_t RNG::getSeed() {
    return seed;
}

uint16_t RNG::next(uint16_t max){	    
    return uint16_t(rand() % max);
}

double RNG::randZeroToOne(){
    return rand() / (RAND_MAX + 1.);
}

bool RNG::trueFalseProb(double limit){
    //std::cout << k <<std::endl;	
    limit = limit - randZeroToOne();
    if(limit > 0) return true;
    return false;
}

uint16_t RNG::next(uint16_t a, uint16_t b){
    return (next(b-a)+a);
}
