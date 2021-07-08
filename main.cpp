//#include "testbench.hpp"
#include "Checksum16Bit.hpp"
#include "CRC16Bit.hpp"
#include "CRC32Bit.hpp"
#include "Packet.hpp"
#include "BernoulliErrorModel.hpp"
#include "GilbertErrorModel.hpp"
#include "PeriodicBurstErrorModel.hpp"

//#include "CRC16Bit.hpp"
#include <iostream>
#include <iomanip>//setprecision setfixed
#include <cmath> //pow
#include <cstdint>
#include <cstdlib>
#include <time.h>
#include <random>

#define CHK 0
#define CRC 1
#define CRC32 2

#define DEBUG false
#define DEBUGCHK16 false
#define DEBUGCRC16 false
#define DEBUGCRC32 false

#define FIXED_FLOAT(p, x) fixed << setprecision(p) <<(x)

using namespace std;

#define TIMES 1000000
#define BER 0.001 //0.001 0.005 0.01 0.02
#define GIL_BURST 2

#define SEED 0x2021

static void test(ErrorModel *model, bool forceError) {
	int detectionFails[3]; //0=checksum, 1=crc 2=crc32 para contar o numero de falhas de detecção
	int bitErrors; //para contar o numero de total bits invertidos
	
	//checksum
	Checksum16Bit check;
	//CRC
	CRC16Bit crc;
	CRC32Bit crc32;
	crc32.generateTable(0x04c11db7);

	long diff[50];
	int devpad[TIMES];
	for (int x = 0; x<20; ++x) diff[x]=0;
	
	cout << "\nN(B)\tf(bit)\tDesvPad\t%\tFY\t%\tCHK\t%\tCRC16\t%\tBOTH\tCRC32\t%\tBOTH" << endl;
	for (int N=pow(2,3); N<pow(2,11); N*=2) {
		detectionFails[CHK] = 0;
		detectionFails[CRC] = 0;
		detectionFails[CRC32] = 0;
		bitErrors = 0;
	RNG *rng = new RNG(N);
	for (int i=0; i<TIMES; ++i) devpad[i]=0;

	int bothUndetected = 0, both32Undetected = 0; //numero de vezes que chk e crc falharam juntos
	long testsWithErrors = 0; //execuções com erros injetados para cada cenario
	for(int i=0; i<TIMES; ++i){
		Packet *pkg = new Packet(N, rng);
		Packet *pkgBefore = pkg->clone();
		//pkg->print('b');

		uint16_t _chk = check.doChecksum(pkg);		
		uint16_t _crc = crc.doCRC(pkg); 
		uint32_t _crc32 = crc32.doCRC(pkg);

		int err = model->injectErrors(pkg, forceError);//true=forceError
		bitErrors+=err;
		devpad[i] = err;
		if (err>0) {
			testsWithErrors++;
						
		}
   
		if (DEBUG){
			cout << endl <<"pkgBefore: "<< endl;
			pkgBefore->print('b');
			cout<<"pkg: "<< endl;
			pkg->print('b');
		}
		bool chkUndetect = false;
		if (err>0 && check.verifyChecksum(pkg, _chk)) {
		   if (DEBUGCHK16) {
			cout << "CHK match - detection failed" << endl;
			cout << check.doChecksum(pkg) << " " 
			<< check.doChecksum(pkgBefore) << " "
			<<_chk << endl;
		   }
		   chkUndetect = true;
		   detectionFails[CHK]++;
		} 		
	        
		if (err>0 && crc.verifyCRC(pkg, _crc)) {
		   if (DEBUGCRC16) {
			cout << endl << "CRC16 match - detection failed" << endl;
			cout << crc.doCRC(pkg) << " " 
			<< crc.doCRC(pkgBefore) << " "
			<<_crc << endl;
		   }
                   if (chkUndetect == true) bothUndetected++;
		   detectionFails[CRC]++;
		}

		if (err>0 && crc32.verifyCRC(pkg, _crc32)) {
		   if (DEBUGCRC32) {
			cout << "CRC32 match - detection failed" << endl;
			cout << crc32.doCRC(pkg) << " " 
			<< crc32.doCRC(pkgBefore) << " "
			<<_crc32 << endl;
		   }
		   if (chkUndetect == true) both32Undetected++;                    
		   detectionFails[CRC32]++;
		}		
		delete pkg;
		delete pkgBefore;
	}
	double errp = (double)bitErrors/TIMES;
	//desvio padrao
	double sum = 0.0;
	for (long x=0; x<TIMES; ++x)
	    sum+= pow(devpad[x]-errp, 2);
	double dpad = sqrt(sum/TIMES);

	cout<< dec<<N<<"\t"
    <<FIXED_FLOAT(2,errp)<<"\t" //media de bits invertidos
	<<FIXED_FLOAT(2,dpad)<<"\t" //devpad
    <<FIXED_FLOAT(4,(errp*100.0)/(N*8))<<"\t" //% bits invertidos no pacote
	<<testsWithErrors<<"\t"
	<<FIXED_FLOAT(2,(testsWithErrors*100.0)/TIMES)<<"\t"
    <<detectionFails[CHK]<<"\t"
    <<FIXED_FLOAT(4,detectionFails[CHK]*100.0/testsWithErrors)<<"\t" //falhas de detecção checksum
	<<detectionFails[CRC]<<"\t"
	<<FIXED_FLOAT(4,detectionFails[CRC]*100.0/testsWithErrors)<<"\t"//falhas de detecção crc		
	<<bothUndetected<<"\t"
	<<detectionFails[CRC32]<<"\t"
	<<FIXED_FLOAT(4,detectionFails[CRC32]*100.0/testsWithErrors)<<"\t"//falhas de detecção crc32
	<<both32Undetected<<endl;
    //for (int x=0; x<20;++x) cout<<"["<<x<<"] = "<<diff[x]<<endl;
        delete rng;
	
	}//end for N
		
}

void testRoutine(){
	
	//teste bernoulli com BER = 0.001, 0.002, 0.01, 0.02
	RNG* rng = new RNG(SEED);
	
	ErrorModel *ber = new BernoulliErrorModel(0.001, rng);
	cout << endl << endl <<"\nBER= "<<0.001<<endl;
	test(ber, true);
	delete ber;	

	delete rng;
	rng = new RNG(SEED);
	
	ber = new BernoulliErrorModel(0.002, rng);
	cout << endl << endl <<"\nBER= "<<0.002<<endl;
	test(ber, true);
	delete ber;	

	delete rng;
	rng = new RNG(SEED);
	
	ber = new BernoulliErrorModel(0.01, rng);
	cout << endl << endl <<"\nBER= "<<0.01<<endl;
	test(ber, true);
	delete ber;	

	delete rng;
	rng = new RNG(SEED);
	
	ber = new BernoulliErrorModel(0.02, rng);
	cout << endl << endl <<"\nBER= "<<0.02<<endl;
	test(ber, true);
	delete ber;	

	//teste gilbert com rajadas de 2 e 3 bits
	delete rng;
    rng = new RNG(SEED);
    GilbertErrorModel *gil = new GilbertErrorModel(2, 0.001, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.001<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;
	test(gil, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(2, 0.002, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.002<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test(gil, true);
	delete gil;

	delete rng;
	rng = new RNG(SEED);
    gil = new GilbertErrorModel(2, 0.01, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.01<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test(gil, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(3, 0.001, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.001<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test(gil, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(3, 0.002, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.002<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test(gil, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(3, 0.01, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.01<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test(gil, true);
	delete gil;

	//teste periodico
	delete rng;
    rng = new RNG(SEED);
    PeriodicBurstErrorModel *per = new PeriodicBurstErrorModel(1,16, rng);
	cout << "\nPER= "<<1<<", "<<16<<endl;
	test(per, true);
	delete per;

	delete rng;
	rng = new RNG(SEED);
    per = new PeriodicBurstErrorModel(1,3,16,16, rng);
	cout << "\nPER= "<<1<<", "<<3<<", "<<16<<", "<<16<<endl;	
	test(per, true);
	delete per;

	delete rng;
}

void burstVerification(ErrorModel *model, int N) {
	
	long diff[50];
	for (int x = 0; x<20; ++x) diff[x]=0;
	
	RNG *rng = new RNG(N);

	for(int i=0; i<TIMES; ++i){
		Packet *pkg = new Packet(N, rng);
		//pkg->print('b');
		Packet *pk2 = pkg->clone();

		int err = model->injectErrors(pkg, true);//true=forceError

		if (err>0) {
		//pkg->print('b');
 			int k = 0;
			for (uint16_t x =0; x<pkg->getLength()/2; ++x) {
				uint16_t auxiliary = (pkg->getData()[x]^pk2->getData()[x]);			
				for (uint16_t y =15; y<16;--y) {
			            int d = uint16_t(pow(2, y));
				    int r = auxiliary&d;
				    //cout<<bitset<16>(auxiliary)<< "- "<<bitset<16>(d)<< "- "<<bitset<16>(r)<<endl;	
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

	for (int x=0; x<20;++x) cout<<"["<<x<<"] = "<<diff[x]<<endl;
}

static void comparePolynomials32(ErrorModel *model, uint32_t polyA, uint32_t polyB) {
	int detectionFails[2]; //0 = PolyA, 1 = PolyB
	int bitErrors; //para contar o numero de total bits invertidos
	
	CRC32Bit crc32A;
	CRC32Bit crc32B;
	crc32A.generateTable(polyA);
	crc32B.generateTable(polyB);

	long diff[50];
	int devpad[TIMES];
	for (int x = 0; x<20; ++x) diff[x]=0;
	
	cout << "\nN(B)\tf(bit)\tDesvPad\t%\t" << hex << polyA << "\t" << hex << polyB << " \tBOTH" << endl << dec;
	for (int N=pow(2,3); N<pow(2,11); N*=2) {
		detectionFails[0] = 0;
		detectionFails[1] = 0;
		bitErrors = 0;
	RNG *rng = new RNG(N);
	for (int i=0; i<TIMES; ++i) devpad[i]=0;

	int both32Undetected = 0;
	long testsWithErrors = 0; //execuções com erros injetados para cada cenario
	for(int i=0; i<TIMES; ++i){
		Packet *pkg = new Packet(N, rng);

		uint32_t _crcA = crc32A.doCRC(pkg);
		uint32_t _crcB = crc32B.doCRC(pkg);
		
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
	double errp = (double)bitErrors/TIMES;
	//desvio padrao
	double sum = 0.0;
	for (long x=0; x<TIMES; ++x)
	    sum+= pow(devpad[x]-errp, 2);
	double dpad = sqrt(sum/TIMES);

	cout<<N<<"\t"
    <<FIXED_FLOAT(2,errp)<<"\t" //media de bits invertidos
	<<FIXED_FLOAT(2,dpad)<<"\t" //devpad
    <<FIXED_FLOAT(4,(errp*100.0)/(N*8))<<"\t" //% bits invertidos no pacote
    <<detectionFails[0]<<"\t"
	<<detectionFails[1]<<"\t\t"
	<<both32Undetected<<endl;
        delete rng;
	
	}//end for N
		
}

int main(){

	RNG* rng = new RNG(SEED);
    BernoulliErrorModel *ber = new BernoulliErrorModel(0.001, rng);
	cout << endl << endl <<"\nBER= "<<0.001<<endl;
	test(ber, false);
	delete ber;
	delete rng;

	rng = new RNG(SEED);
    ber = new BernoulliErrorModel(0.01, rng);
	cout << endl << endl <<"\nBER= "<<0.01<<endl;
	test(ber, false);
	delete ber;
	delete rng;

	rng = new RNG(SEED);
    GilbertErrorModel *gil = new GilbertErrorModel(2, 0.001, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.001<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;
	test(gil, false);
	delete gil;
	delete rng;

	rng = new RNG(SEED);
    gil = new GilbertErrorModel(2, 0.01, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.01<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test(gil, false);
	delete gil;
	delete rng;

	rng = new RNG(SEED);
    PeriodicBurstErrorModel *per = new PeriodicBurstErrorModel(1,16, rng);
	cout << "\nPER= "<<1<<", "<<16<<endl;
	test(per, false);
	delete per;
	delete rng;

	rng = new RNG(SEED);
    per = new PeriodicBurstErrorModel(1,3,16,16, rng);
	cout << "\nPER= "<<1<<", "<<3<<", "<<16<<", "<<16<<endl;	
	test(per, false);
	delete per;
	delete rng;

	/*
	Packet *pkg = new Packet(8, rng);
	pkg->print('h');
	cout<<endl;
	uint8_t* u = new uint8_t[pkg->getLength()];
	std::memcpy(u, pkg->getData(), pkg->getLength());//static_cast<const uint8_t*>(packet->getData());
	for(int i=0; i<8; i++) cout<< +u[i]<<" ";
	*/


	return 0;
}