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

void TestRoutines::comparePolynomials32(ErrorModel *model, uint32_t polyA, uint32_t polyB){
	int detectionFails[2]; //0 = PolyA, 1 = PolyB
	int bitErrors; //para contar o numero de total bits invertidos
	
	CRC32Bit crc32A(polyA);
	CRC32Bit crc32B(polyB);

	long diff[50];
	int devpad[times];
	for (int x = 0; x<20; ++x) diff[x]=0;
	
	std::cout << "\nN(B)\tf(bit)\tDesvPad\t%\t" << std::hex << polyA << "\t" << std::hex << polyB << " \tBOTH" << std::endl << std::dec;
	for (int N=pow(2,3); N<pow(2,11); N*=2) {
		detectionFails[0] = 0;
		detectionFails[1] = 0;
		bitErrors = 0;
	RNG *rng = new RNG(N);
	for (int i=0; i<times; ++i) devpad[i]=0;

	int both32Undetected = 0;
	long testsWithErrors = 0; //execuções com erros injetados para cada cenario
	for(int i=0; i<times; ++i){
		Packet *pkg = new Packet(N, rng);

		uint32_t _crcA = crc32A.generateVerificationCode(pkg);
		uint32_t _crcB = crc32B.generateVerificationCode(pkg);
		
		int err = model->injectErrors(pkg, true);//true=forceError
		bitErrors+=err;
		devpad[i] = err;

		bool crcA_undetect = false;
		if (err>0 && crc32A.verifyCRC(pkg, _crcA)) {
		    crcA_undetect = true;
		    detectionFails[0]++;
		} 		
	        
		if (err>0 && crc32B.verifyCRC(pkg, _crcB)) {
		    if (crcA_undetect == true) both32Undetected++;                    
		    detectionFails[1]++;
		}		
		delete pkg;
	}
	double errp = (double)bitErrors/times;
	//desvio padrao
	double sum = 0.0;
	for (long x=0; x<times; ++x)
	    sum+= pow(devpad[x]-errp, 2);
	double dpad = sqrt(sum/times);

	std::cout<<N<<"\t"
    <<FIXED_FLOAT(2,errp)<<"\t" //media de bits invertidos
	<<FIXED_FLOAT(2,dpad)<<"\t" //devpad
    <<FIXED_FLOAT(4,(errp*100.0)/(N*8))<<"\t" //% bits invertidos no pacote
    <<detectionFails[0]<<"\t"
	<<detectionFails[1]<<"\t\t"
	<<both32Undetected<<std::endl;
        delete rng;
	
	}//end for N
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
/*
void TestRoutines::paperTestTemplate(ErrorModel *model, uint32_t CRC_32, bool forceError) {
	//int detectionFails[3]; //0=checksum, 1=crc 2=crc32 para contar o numero de falhas de detecção
	unsigned int DF_checksum, DF_CRC16, DF_CRC32; //DF = Detection Fail
	int bitErrors; //para contar o numero de total bits invertidos
	
	Checksum16Bit check;
	CRC16Bit crc(0x1021);
	CRC32Bit crc32(CRC_32);

	//long diff[50];
	int devpad[times];
	//for (int x = 0; x<20; ++x) diff[x]=0;
	
	std::cout << "\nN(B)\tf(bit)\tDesvPad\t%\tFY\t%\tCHK\t%\tCRC16\t%\tBOTH\tCRC32\t%\tBOTH" << std::endl;
	for (int N=pow(2,3); N<pow(2,11); N*=2) {
		DF_checksum = 0;
		DF_CRC16 = 0;
		DF_CRC32 = 0;
		bitErrors = 0;
	RNG *rng = new RNG(N);
	for (int i=0; i<times; ++i) devpad[i]=0;

	int bothUndetected = 0, both32Undetected = 0; //numero de vezes que chk e crc falharam juntos
	long testsWithErrors = 0; //execuções com erros injetados para cada cenario
	for(int i=0; i<times; ++i){
		Packet *pkg = new Packet(N, rng);
		//pkg->print('b');


		uint16_t _chk = check.generateVerificationCode(pkg);		
		uint16_t _crc = crc.generateVerificationCode(pkg); 
		uint32_t _crc32 = crc32.generateVerificationCode(pkg);
		
		int err = model->injectErrors(pkg, forceError);//true=forceError
		bitErrors+=err;
		devpad[i] = err;
		if (err>0) {
			testsWithErrors++;
						
		}
                

		if (debug) std::cout<<"err: "<<err<<std::endl;
		bool chkUndetect = false;
		if (err>0 && check.verifyChecksum(pkg, _chk)) {
		   if (debug) {
			std::cout << "CHK match - detection failed" << std::endl;
		   	pkg->print('b');
		   }
		   chkUndetect = true;
		   DF_checksum++;
		} 		
	        
		if (err>0 && crc.verifyCRC(pkg, _crc)) {
		   if (debug) {
			std::cout << "CRC match - detection failed" << std::endl;
			pkg->print('b');
		   }
           if (chkUndetect == true) bothUndetected++;
		   DF_CRC16++;
		}

		if (err>0 && crc32.verifyCRC(pkg, _crc32)) {
		   if (debug) {
			std::cout << "CRC32 match - detection failed" << std::endl;
			pkg->print('b');
		   }
		   if (chkUndetect == true) both32Undetected++;                    
		   DF_CRC32++;
		}		
		delete pkg;
	}
	double errp = (double)bitErrors/times;
	//desvio padrao
	double sum = 0.0;
	for (long x=0; x<times; ++x)
	    sum+= pow(devpad[x]-errp, 2);
	double dpad = sqrt(sum/times);

	std::cout<<N<<"\t"
    <<FIXED_FLOAT(2,errp)<<"\t" //media de bits invertidos
	<<FIXED_FLOAT(2,dpad)<<"\t" //devpad
    <<FIXED_FLOAT(4,(errp*100.0)/(N*8))<<"\t" //% bits invertidos no pacote
	<<testsWithErrors<<"\t"
	<<FIXED_FLOAT(2,(testsWithErrors*100.0)/times)<<"\t"
    <<DF_checksum<<"\t"
    <<FIXED_FLOAT(4,DF_checksum*100.0/testsWithErrors)<<"\t" //falhas de detecção checksum
	<<DF_CRC16<<"\t"
	<<FIXED_FLOAT(4,DF_CRC16*100.0/testsWithErrors)<<"\t"//falhas de detecção crc		
	<<bothUndetected<<"\t"
	<<DF_CRC32<<"\t"
	<<FIXED_FLOAT(4,DF_CRC32*100.0/testsWithErrors)<<"\t"//falhas de detecção crc32
	<<both32Undetected<<std::endl;
    //for (int x=0; x<20;++x) cout<<"["<<x<<"] = "<<diff[x]<<endl;
        delete rng;
	
	}//end for N
		
}
*/