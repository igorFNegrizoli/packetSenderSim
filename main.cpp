#include "Checksum16Bit.hpp"
#include "CRC16Bit.hpp"
#include "CRC32Bit.hpp"
#include "Packet.hpp"
#include "BernoulliErrorModel.hpp"
#include "GilbertErrorModel.hpp"
#include "PeriodicBurstErrorModel.hpp"
#include "TestRoutines.hpp"

#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>

#define DEBUG false
#define TIMES 1000000
#define SEED 0x2021

using namespace std;

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
}

#include <bitset>
int main(){
	
	/*
	RNG* rng = new RNG(SEED);
    GilbertErrorModel *gil = new GilbertErrorModel(3, 0.01, rng);
	cout << endl << endl << "GIL= "<<3<<", "<<0.01<<" p="<<gil->getP()<<", q="<<gil->getQ()<<endl;	
	comparePolynomials32(gil, 0x04C11DB7, 0xc9d204f5);
	delete gil;
	delete rng;
	*/

	//execTestRoutine();
	
	/*
	Packet *pkg = new Packet(8, rng);
	pkg->print('h');
	cout<<endl;
	uint8_t* u = new uint8_t[pkg->getLength()];
	std::memcpy(u, pkg->getData(), pkg->getLength());//static_cast<const uint8_t*>(packet->getData());
	for(int i=0; i<8; i++) cout<< +u[i]<<" ";
	*/

	/*
	RNG* rng = new RNG(SEED);
	VerificationAlgorithm* alg = new Checksum16Bit();
	GilbertErrorModel *gil = new GilbertErrorModel(3, 0.01, rng);
	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	test->comparePolynomials32(gil, 0x04C11DB7, 0xc9d204f5);
	*/

	/*
	RNG* rng = new RNG(SEED);
	//VerificationAlgorithm* alg = new CRC16Bit(0x1021);
	GilbertErrorModel *gil = new GilbertErrorModel(3, 0.01, rng);
	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	test->paperTestTemplate(gil, 0x04C11DB7, true);
	*/

	/*
	RNG* rng = new RNG(SEED);
	VerificationAlgorithm* alg = new Checksum16Bit();
	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	test->executionTimeTest(rng, alg);
	*/

	/*
	RNG* rng = new RNG(SEED);
	Packet *pkg = new Packet(512, rng);
	//pkg->print('h');
	//CRC32Bit* alg = new CRC32Bit(0x04C11DB7);
	//std::cout << std::hex << alg->generateVerificationCode(pkg) << std::endl;
	TestRoutines* test = new TestRoutines(TIMES, DEBUG);
	GilbertErrorModel *gil = new GilbertErrorModel(3, 0.01, rng);
	test->comparePolynomials32(gil, 0x04C11DB7, 0xC9D204F5);
	*/

	
	uint8_t* arr = (uint8_t*)malloc(8);
	uint16_t* arr16 = (uint16_t*)arr;

	for(uint8_t i=0; i<8; i++) arr[i] = i;
	for(uint8_t i=0; i<8; i++) std::cout << std::hex << (int)arr[i] << " ";

	std::cout << std::endl;

	for(uint8_t i=0; i<4; i++) std::cout << (int)arr16[i] << " ";

	std::cout << std::endl;
	

	return 0;
}