#include "Checksum16Bit.hpp"
#include "Checksum32Bit.hpp"
#include "CRC16Bit.hpp"
#include "CRC32Bit.hpp"

#include "Fletcher16Bit.hpp"
#include "Adler16Bit.hpp"

#include "Adler32Bit.hpp"
#include "Fletcher32Bit.hpp"
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
	VerificationAlgorithm *algs[13];
	ErrorModel *errs[21];
	ErrorModel *errs2[5];
	
	algs[0] = new Checksum16Bit();
	algs[1] = new Checksum32Bit();
	algs[2] = new CRC16Bit(0x1021);
	algs[3] = new CRC16Bit(0x8d95); //koopman
	algs[4] = new CRC32Bit(0x04C11DB7);
	algs[5] = new CRC32Bit(0xc9d204f5); //koopman
	algs[6] = new CRC32Bit(0x973afb51); //koopman
	algs[7] = new Fletcher16Bit(true);
	algs[8] = new Fletcher16Bit(false);
	algs[9] = new Adler16Bit();
	algs[10] = new Fletcher32Bit(true);
	algs[11] = new Fletcher32Bit(false);
	algs[12] = new Adler32Bit();

	
	errs[0] = new SparseBurstsErrorModel(0.001, 2, 8, rng);
	errs[1] = new SparseBurstsErrorModel(0.0001, 16, 32, rng);
	errs[2] = new SparseBurstsErrorModel(0.0001, 32, 64, rng);
	errs[3] = new SparseBurstsErrorModel(0.0001, 32, 64, rng);
	errs[4] = new SparseBurstsErrorModel(0.0001, 64, 128, rng);
	errs[5] = new SparseBurstsErrorModel(0.0001, 64, 128, rng);
	errs[6] = new BernoulliErrorModel(0.01, rng);
	errs[7] = new BernoulliErrorModel(0.001, rng);
	errs[8] = new BernoulliErrorModel(0.0001, rng);
	errs[9] = new GilbertErrorModel(2, 0.001, rng);
	errs[10] = new GilbertErrorModel(2, 0.0001, rng);
	errs[11] = new GilbertErrorModel(8, 0.001, rng);
	errs[12] = new GilbertErrorModel(8, 0.0001, rng);
	errs[13] = new GilbertErrorModel(16, 0.001, rng);
	errs[14] = new GilbertErrorModel(16, 0.0001, rng);
	errs[15] = new GilbertErrorModel(32, 0.001, rng);
	errs[16] = new GilbertErrorModel(32, 0.0001, rng);
	errs[17] = new PeriodicBurstErrorModel(1,16, rng);
	errs[18] = new PeriodicBurstErrorModel(1,3,16,16, rng);
	errs[19] = new PeriodicBurstErrorModel(8,16,32,64, rng);
	errs[20] = new PeriodicBurstErrorModel(16,32,128,256, rng);
	

	errs2[0] = new SparseBurstsErrorModel(0.0001, 32, 64, rng);
	errs2[1] = new BernoulliErrorModel(0.0001, rng);
	errs2[2] = new GilbertErrorModel(8, 0.0001, rng);
	errs2[3] = new GilbertErrorModel(16, 0.0001, rng);
	errs2[4] = new PeriodicBurstErrorModel(8,16,32,64, rng);

	TestRoutines* test = new TestRoutines(TIMES, DEBUG);

	test->genericTest(algs, errs, 13, 21, rng);
<<<<<<< HEAD
	test->executionTimeTest(rng, algs, 13);
	
=======
	std::cout << std::endl << std::endl;
	test->compareTwoAlgorithms(errs2, 5, algs[1], algs[4], rng);
	std::cout << std::endl << std::endl;
	test->compareTwoAlgorithms(errs2, 5, algs[1], algs[10], rng);
	std::cout << std::endl << std::endl;
	test->compareTwoAlgorithms(errs2, 5, algs[1], algs[11], rng);
	std::cout << std::endl << std::endl;
	test->compareTwoAlgorithms(errs2, 5, algs[1], algs[12], rng);

>>>>>>> f0d96f29dad2eb87b28e9022bb6bce39613a17a0
	delete rng;
	return 0;
}