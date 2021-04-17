#include "testbench.hpp"
#include "checksum16Bit.hpp"

testBench::testBench(){
	detectionFails = 0;
	totalPackets = 1000;
	len = 16;
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
		checksum16Bit pkg(this->len);
		//pkg.printPacket('h');
		pkg.burstErrorPeriodicModel(80,80,2,2);
		//pkg.printPacket('h');
		if(pkg.verifyChecksum()){
			++this->detectionFails;
			//pkg.printPacket('b');
		}
	}

	return this->detectionFails;
}

testBench::~testBench(){

}
