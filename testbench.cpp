#include "testbench.hpp"
#include "packet.hpp"
//#include <string>
//#include <iostream>
//#include <fstream>

//using namespace std;
//std::ofstream outfile;

testBench::testBench(){
	detectionFails = 0;
	totalPackets = 1000;
	len = 8;
	protocol = "udp";
}

testBench::testBench(string protocol_, uint64_t totalPackets_, uint16_t len_){
	detectionFails = 0;
	totalPackets = totalPackets_;
	len = len_;
	protocol = protocol_;
}

uint16_t testBench::doTest(){
	this->detectionFails = 0;
	bool errorFlag;
	uint16_t burst = 2;
	double plRate = 0.07;

	for(uint64_t i=0; i<this->totalPackets; ++i){
		packet pkg(this->len);
		//pkg.printPacket('h');
		pkg.gilbertModel(burst, plRate);
		//pkg.printPacket('h');
		if(pkg.verifyChecksum()){
			++this->detectionFails;
			//pkg.printPacket('b');
		}
	}
	//outfile.open("logs.txt", std::ios_base::app);
	//outfile << "<Bernoulli Model> <len> " << this->len << " <Prob> " << prob << " <totalPackets> " << this->totalPackets << " <detectionFails> " << this->detectionFails;
	//outfile << endl;
	//outfile.close();
	return this->detectionFails;
}

testBench::~testBench(){

}
