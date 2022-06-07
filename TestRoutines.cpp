#include "Checksum16Bit.hpp"
#include "CRC16Bit.hpp"
#include "CRC32Bit.hpp"
#include "Packet.hpp"
#include "BernoulliErrorModel.hpp"
#include "GilbertErrorModel.hpp"
#include "PeriodicBurstErrorModel.hpp"
#include "TestRoutines.hpp"

#include <cstdint>
#include <cmath>
#include <chrono>
#include <iomanip>//setprecision setfixed

#define FIXED_FLOAT(p, x) std::fixed << std::setprecision(p) <<(x)

TestRoutines::TestRoutines(uint32_t times_, bool debug_){
	this->times = times_;
	this->debug = debug_;
}

void TestRoutines::compareTwoAlgorithms(ErrorModel **errs, uint8_t lenErr, VerificationAlgorithm* algA, VerificationAlgorithm* algB, RNG* rng){
	
	for(uint8_t errCounter=0; errCounter<lenErr; errCounter++){
		std::cout << std::endl;
		errs[errCounter]->printAttributes();

		int detectionFails[3]; //0 = PolyA, 1 = PolyB, 2 = BOTH
		std::cout << "\nN(B)\tf(bit)\t%\t"
		<< algA->getAlgName() << "\t"
		<< algB->getAlgName() << "\t"
		<< "BOTH" << std::endl;
		for (uint16_t N=pow(2,3); N<pow(2,11); N*=2){
			detectionFails[0] = 0;
			detectionFails[1] = 0;
			detectionFails[2] = 0;
			uint32_t bitErrors = 0;

			for(int i=0; i<times; ++i){
				Packet *pkg = new Packet(N, rng);

				uint32_t verCodeA = algA->generateVerificationCode(pkg);
				uint32_t verCodeB = algB->generateVerificationCode(pkg);
				
				int err = errs[errCounter]->injectErrors(pkg, true);//true=forceError
				bitErrors =+ err;

				bool algA_undetect = false;
				if (err>0 && verCodeA == algA->generateVerificationCode(pkg)) {
					algA_undetect = true;
					detectionFails[0]++;
				} 		
					
				if (err>0 && verCodeB == algB->generateVerificationCode(pkg)) {
					if (algA_undetect == true) detectionFails[2]++;                    
					detectionFails[1]++;
				}		
				delete pkg;
			}
			std::cout << N << "\t"
			<< FIXED_FLOAT(2, (double)bitErrors/times) << "\t"
			<< FIXED_FLOAT(2, (((double)bitErrors/times)*100.0)/(N*8)) << "\t"
			<< detectionFails[0] << "\t"
			<< detectionFails[1] << "\t"
			<< detectionFails[2];
			std::cout << std::endl;

		}
	}
}

void TestRoutines::executionTimeTest(RNG* rng, VerificationAlgorithm* alg){
	for (int N=pow(2,3); N<pow(2,11); N*=2){
		long unsigned int mediaTempo = 0;
		//std::chrono::high_resolution_clock mediaTempo;

		for(uint32_t i=0; i<times; i++){
			//if(i%50000 == 0) std::cout << "packet number: " << i << std::endl;
			Packet *pkg = new Packet(N, rng);
			auto start = std::chrono::high_resolution_clock::now();	
			alg->generateVerificationCode(pkg);
			auto stop = std::chrono::high_resolution_clock::now();
			mediaTempo += std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count();
			//mediaTempo = mediaTempo + stop - start;
			delete pkg;
		}
		std::cout << N << " - " << mediaTempo << std::endl;
	}
}

void TestRoutines::burstVerification(ErrorModel *model, int N){
	long diff[50];
	for (int x = 0; x<20; ++x) diff[x]=0;
	
	RNG *rng = new RNG(N);

	for(int i=0; i<times; ++i){
		Packet *pkg = new Packet(N, rng);
		//pkg->print('b');
		Packet *pk2 = pkg->clone();

		int err = model->injectErrors(pkg, true);//true=forceError

		if (err>0) {
		//pkg->print('b');
 			int k = 0;
			for (uint16_t x =0; x<pkg->getLength(); ++x) {
				uint16_t auxiliary = (((uint8_t*)pkg->getData())[x]^((uint8_t*)pk2->getData())[x]);			
				for (uint16_t y = 7; y<8;--y) {
			            int d = uint16_t(pow(2, y));
				    int r = auxiliary&d;
				    //cout<<bitset<16>(auxiliary)<< "- "<<bitset<16>(d)<< "- "<<bitset<16>(r)<<std::endl;	
				    if (r >0) k++;
				    else { 
					diff[k]++; 
					if (k>0) diff[0]++;
					k=0;					
                    }
				}
			}			
		}
	}

	for (int x=0; x<20;++x) std::cout<<"["<<x<<"] = "<<diff[x]<<std::endl;
}

void TestRoutines::genericTest(VerificationAlgorithm** algs, ErrorModel** errs, uint8_t lenAlg, uint8_t lenErr, RNG* rng){
	
	for(uint8_t errCounter=0; errCounter<lenErr; errCounter++){

		std::cout << std::endl;
		errs[errCounter]->printAttributes();

		uint32_t DFs[lenAlg]; //detection fail for each verifiation algorithm

		std::cout << "\nN(B)\tf(bit)\t%\t";
		for(uint8_t i=0; i<lenAlg; i++) std::cout << algs[i]->getAlgName() << "\t";
		std::cout << std::endl;

		for (uint16_t N=pow(2,3); N<pow(2,11); N*=2){

			for (uint8_t x=0; x<lenAlg; x++) 
				DFs[x] = 0;

			uint16_t testsWithErrors = 0;
			uint32_t bitErrors = 0;

			for(uint32_t i=0; i<times; i++){
				
				Packet *pkg = new Packet(N, rng);

				uint32_t chks[lenAlg]; //checksums before error injection


				for(uint8_t algCounter=0; algCounter<lenAlg; algCounter++){
					chks[algCounter] = algs[algCounter]->generateVerificationCode(pkg);
				}

				int err = errs[errCounter]->injectErrors(pkg, true);

				bitErrors += err;

				if (err>0) testsWithErrors++;			

				for(uint8_t algCounter=0; algCounter<lenAlg; algCounter++){
					if(err>0 && (algs[algCounter]->generateVerificationCode(pkg) == chks[algCounter])){
						DFs[algCounter]++;
					}
				}
				
				delete pkg;
			}
			std::cout << N << "\t"
			<< FIXED_FLOAT(2, (double)bitErrors/times) << "\t"
			<< FIXED_FLOAT(2, (((double)bitErrors/times)*100.0)/(N*8)) << "\t";
			for(uint8_t i=0; i<lenAlg; i++) std::cout << DFs[i] << "\t";
			std::cout << std::endl;
		}
	}
}