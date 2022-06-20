#ifndef Included_TestRoutines_HPP
#define Included_TestRoutines_HPP

class TestRoutines{
private:
	uint32_t times;
	bool debug;

public:
	TestRoutines(uint32_t times_, bool debug_);

	void executionTimeTest(RNG* rng, VerificationAlgorithm** algs, uint8_t lenAlg);
	void compareTwoAlgorithms(ErrorModel **errs, uint8_t lenErr, VerificationAlgorithm* algA, VerificationAlgorithm* algB, RNG* rng);
	void burstVerification(ErrorModel *model, int N);
	void paperTestTemplate(ErrorModel *model, uint32_t CRC_32, bool forceError);
	void genericTest(VerificationAlgorithm** algs, ErrorModel** errs, uint8_t lenAlg, uint8_t lenErr, RNG* rng);
};

#endif