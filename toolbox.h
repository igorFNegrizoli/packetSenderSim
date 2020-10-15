#include <stdlib.h>
#include <time.h>

uint16_t randomnum(uint16_t max) {
	srand (clock());
    return (rand() % max);

}

