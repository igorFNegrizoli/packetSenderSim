#include "RNG.hpp"
#include <cstdint>
#include <cstdlib>
#include <time.h> 
#include <iostream>
#include <random>

RNG::RNG() {
   this->seed = (uint16_t)clock();
   //srand(seed);
}

RNG::RNG(uint16_t seed) {
  this->seed = seed;
  this->gen = std::mt19937(seed);
  //this->dist = nullptr;
  //srand(seed);
}

uint16_t RNG::getSeed() {
    return seed;
}


uint16_t RNG::next(uint16_t max){
    return next(0x0000, max-1);
}

double RNG::randZeroToOne(){
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(this->gen);
}

bool RNG::trueFalseProb(double limit){
    //std::cout << k <<std::endl;	
    limit = limit - randZeroToOne();
    if(limit > 0) return true;
    return false;
}

uint16_t RNG::next(uint16_t a, uint16_t b){
    //if(this->dist == nullptr){
    //  this->dist = new std::uniform_int_distribution<uint16_t>(a, b);
    //}
    //std::mt19937 genObj = std::mt19937(* gen);
    std::uniform_int_distribution<uint16_t> dist(a, b);
    return dist(this->gen);
}

//https://devdocs.io/cpp/header/random