#include "testbench.h"
#include "udp.h"
#include <string>
#include <iostream>

using namespace std;

testBench::testBench(){
	detectionFails = 0;
	totalPackets = 100;
	len = 8;
	protocol = "udp";
}

testBench::testBench(string protocol_, uint16_t totalPackets_, uint16_t len_){
	detectionFails = 0;
	totalPackets = totalPackets_;
	len = len_;
	protocol = protocol_;
}

uint16_t testBench::doTest(){
	bool errorFlag;
	for(uint16_t i=0; i<this->totalPackets; ++i){
		udpPacket pkg(this->len);
		//pkg.printPacket('h');
		pkg.bernoulliModel(0.1);
		//pkg.printPacket('h');
		if(pkg.verifyChecksum()){
			++this->detectionFails;
			pkg.printPacket('b');
		}
	}
	return this->detectionFails;
}

testBench::~testBench(){

}
