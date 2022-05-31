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
#define SEED 0x2021

using namespace std;
/*
void execTestRoutine(){

	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	
	//teste bernoulli com BER = 0.001, 0.002, 0.01, 0.02
	RNG* rng = new RNG(SEED);
	
	ErrorModel *ber = new BernoulliErrorModel(0.001, rng);
	cout << endl << endl <<"\nBER= "<<0.001<<endl;
	test->paperTestTemplate(ber, 0xc9d204f5, true);
	//test(ber, true);
	delete ber;	

	delete rng;
	rng = new RNG(SEED);
	
	ber = new BernoulliErrorModel(0.002, rng);
	cout << endl << endl <<"\nBER= "<<0.002<<endl;
	test->paperTestTemplate(ber, 0xc9d204f5, true);
	delete ber;	

	delete rng;
	rng = new RNG(SEED);
	
	ber = new BernoulliErrorModel(0.01, rng);
	cout << endl << endl <<"\nBER= "<<0.01<<endl;
	test->paperTestTemplate(ber, 0xc9d204f5, true);
	delete ber;	

	delete rng;
	rng = new RNG(SEED);
	
	ber = new BernoulliErrorModel(0.02, rng);
	cout << endl << endl <<"\nBER= "<<0.02<<endl;
	test->paperTestTemplate(ber, 0xc9d204f5, true);
	delete ber;	

	//teste gilbert com rajadas de 2 e 3 bits
	delete rng;
    rng = new RNG(SEED);
    GilbertErrorModel *gil = new GilbertErrorModel(2, 0.001, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.001<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;
	test->paperTestTemplate(gil, 0xc9d204f5, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(2, 0.002, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.002<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test->paperTestTemplate(gil, 0xc9d204f5, true);
	delete gil;

	delete rng;
	rng = new RNG(SEED);
    gil = new GilbertErrorModel(2, 0.01, rng);
	cout << endl << endl << "GIL= "<<2<<", "<<0.01<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test->paperTestTemplate(gil, 0xc9d204f5, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(3, 0.001, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.001<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test->paperTestTemplate(gil, 0xc9d204f5, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(3, 0.002, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.002<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test->paperTestTemplate(gil, 0xc9d204f5, true);
	delete gil;

	delete rng;
    rng = new RNG(SEED);
    gil = new GilbertErrorModel(3, 0.01, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.01<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	test->paperTestTemplate(gil, 0xc9d204f5, true);
	delete gil;

	//teste periodico
	delete rng;
    rng = new RNG(SEED);
    PeriodicBurstErrorModel *per = new PeriodicBurstErrorModel(1,16, rng);
	cout << "\nPER= "<<1<<", "<<16<<endl;
	test->paperTestTemplate(per, 0xc9d204f5, true);
	delete per;

	delete rng;
	rng = new RNG(SEED);
    per = new PeriodicBurstErrorModel(1,3,16,16, rng);
	cout << "\nPER= "<<1<<", "<<3<<", "<<16<<", "<<16<<endl;	
	test->paperTestTemplate(per, 0xc9d204f5, true);
	delete per;

	delete rng;
}*/

#include <bitset>
int main(){
	
	RNG* rng = new RNG(SEED);
	VerificationAlgorithm *algs[7];
	SparseBurstsErrorModel *spb = new SparseBurstsErrorModel(0.1, 2, 8, rng);
	algs[0] = new Checksum16Bit();
	algs[1] = new Checksum32Bit();
	algs[2] = new CRC32Bit(0xc9d204f5);
	algs[3] = new CRC32Bit(0x04C11DB7);
	algs[4] = new FletcherAdler32Bit(0,0,65535);
	algs[5] = new FletcherAdler32Bit(0,0,65536);
	algs[6] = new FletcherAdler32Bit(1,0,65521);
	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	test->genericTest(spb, algs, 7, true);
	
	

	/*
	//inicializa arr
	int arraySize = 8;
	uint8_t* arr = (uint8_t*)malloc(arraySize);
	uint16_t* arr16 = (uint16_t*)malloc(arraySize);//arr;

	//atribui valores e printa arr8
	for(uint8_t i=0; i<8; i++) arr[i] = i;
	for(uint8_t i=0; i<8; i++) std::cout << std::hex << (int)arr[i] << " ";

	std::cout << std::endl;
	
	//inicializa e atribui arr16
	arr16 = (uint16_t*)arr;	
	for(uint8_t i=0; i<4; i++) std::cout << (int)arr16[i] << " ";

	std::cout << std::endl;
	
	//converte little endian
	int i = 0;
	while(i<arraySize){
		uint16_t value = 0;
		for(int j = 0; j<sizeof(uint16_t); j++, i++){
			std::cout << " Value: " << value;
			value += ((uint16_t)arr[i]) << (8*(sizeof(uint16_t)-j-1));
			std::cout << " -> " << value << std::endl;
		}
		arr16[(i/2)-1] = value;
	}

	for(uint8_t i=0; i<4; i++) std::cout << (int)arr16[i] << " ";

	std::cout << std::endl;
	*/

	/*
	RNG* rng = new RNG(SEED);
	Packet *pkg = new Packet(8, rng);
	uint16_t* data16 = (uint16_t*)pkg->char2BigEndian(2);
	uint32_t* data32 = (uint32_t*)pkg->char2BigEndian(4);

	for(int i=0; i<8; i++)
		std::cout << std::hex << (int)((uint8_t*)pkg->getData())[i] << " ";
	std::cout << std::endl;

	for(int i=0; i<4; i++)
		std::cout << std::hex << data16[i] << " ";
	std::cout << std::endl;

	for(int i=0; i<2; i++)
		std::cout << std::hex << data32[i] << " ";
	std::cout << std::endl;
	*/

	/*
	RNG* rng = new RNG(SEED);
	Packet *pkg = new Packet(8, rng);
	pkg->print('h');
	//pkg->injectErrorInChunk(8);
	GilbertErrorModel *gil = new GilbertErrorModel(3, 0.1, rng);
	std::cout << gil->injectErrors(pkg, true) << std::endl;
	pkg->print('h');
	*/

	return 0;
}