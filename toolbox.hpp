#include <stdlib.h>
#include <time.h>
#include <random>

uint16_t randomnum(uint16_t max) {
	srand (clock());
    return (rand() % max);

}

double randZeroToOne(){
	srand (clock());
    return rand() / (RAND_MAX + 1.);
}

bool trueFalseProb(double limit){
	limit = limit - randZeroToOne();
	if(limit > 0) return true;
	return false;
}

uint16_t randomIntInterval(uint16_t a, uint16_t b){
	std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(a, b); // define the range
	return distr(gen);
}