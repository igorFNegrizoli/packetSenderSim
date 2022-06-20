#ifndef Included_TestRoutines_HPP
#define Included_TestRoutines_HPP

class TestRoutines{
private:
	uint32_t times;
	bool debug;

public:
	TestRoutines(uint32_t times_, bool debug_);
	void comparePolynomials32(ErrorModel *model, uint32_t polyA, uint32_t polyB);
	void executionTimeTest(RNG* rng, VerificationAlgorithm** algs, uint8_t lenAlg);
	void burstVerification(ErrorModel *model, int N);
	void paperTestTemplate(ErrorModel *model, uint32_t CRC_32, bool forceError);
	void genericTest(VerificationAlgorithm** algs, ErrorModel** errs, uint8_t lenAlg, uint8_t lenErr, RNG* rng);
};

#endif