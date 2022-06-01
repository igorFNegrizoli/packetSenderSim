#include "Checksum16Bit.hpp"
#include "Checksum32Bit.hpp"
#include "CRC16Bit.hpp"
#include "CRC32Bit.hpp"
#include "FletcherAdler16Bit.hpp"
#include "FletcherAdler32Bit.hpp"
#include "Packet.hpp"
#include "BernoulliErrorModel.hpp"
#include "GilbertErrorModel.hpp"
#include "PeriodicBurstErrorModel.hpp"
#include "SparseBurstsErrorModel.hpp"
#include "TestRoutines.hpp"

#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>

#define DEBUG false
#define TIMES 1000000
#define SEED 0x1234

int main(){
	
	RNG* rng = new RNG(SEED);
	VerificationAlgorithm *algs[8];
	ErrorModel *errs[14];
	
	algs[0] = new Checksum16Bit();
	algs[1] = new Checksum32Bit();
	algs[2] = new CRC32Bit(0xc9d204f5);
	algs[3] = new CRC32Bit(0x04C11DB7);
	algs[3] = new CRC16Bit(0x1021);
	algs[4] = new FletcherAdler32Bit(0,0,65535);
	algs[5] = new FletcherAdler32Bit(0,0,65536);
	algs[6] = new FletcherAdler32Bit(1,0,65521);

	errs[0] = new SparseBurstsErrorModel(0.1, 2, 8, rng);
	errs[1] = new SparseBurstsErrorModel(0.01, 2, 8, rng);
	errs[2] = new SparseBurstsErrorModel(0.01, 16, 32, rng);
	errs[3] = new SparseBurstsErrorModel(0.001, 32, 64, rng);
	errs[4] = new BernoulliErrorModel(0.01, rng);
	errs[5] = new BernoulliErrorModel(0.001, rng);
	errs[6] = new GilbertErrorModel(2, 0.001, rng);
	errs[7] = new GilbertErrorModel(2, 0.002, rng);
	errs[8] = new GilbertErrorModel(3, 0.001, rng);
	errs[9] = new GilbertErrorModel(3, 0.002, rng);
	errs[10] = new PeriodicBurstErrorModel(1,16, rng);
	errs[11] = new PeriodicBurstErrorModel(1,3,16,16, rng);
	errs[12] = new PeriodicBurstErrorModel(16,32,128,256, rng);
	errs[13] = new PeriodicBurstErrorModel(8,16,32,64, rng);

	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	test->genericTest(algs, errs, 7, 14, rng);
	
	delete rng;
	return 0;
}