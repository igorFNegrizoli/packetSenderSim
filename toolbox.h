#include <stdlib.h>
#include <time.h>

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