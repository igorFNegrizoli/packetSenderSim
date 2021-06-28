#ifndef Included_RNG_HPP
#define Included_RNG_HPP
#include <cstdint>
#include <random>

class RNG {

   private:
        uint16_t seed;
        std::mt19937 gen;
	
   public:
	RNG();
	RNG(uint16_t seed);
	uint16_t getSeed();

	uint16_t next(uint16_t max);
	uint16_t next(uint16_t a, uint16_t b);
	//void attSeed();

	double randZeroToOne();
	bool trueFalseProb(double limit);
};
#endif

