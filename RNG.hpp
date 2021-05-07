#ifndef Included_RNG_HPP
#define Included_RNG_HPP
#include <cstdint>

class RNG {

   private:
        uint16_t seed;
	
   public:
	RNG();
	RNG(uint16_t seed);
	uint16_t getSeed();

	uint16_t next(uint16_t max);
	uint16_t next(uint16_t a, uint16_t b);

	double randZeroToOne();
	bool trueFalseProb(double limit);
};
#endif

