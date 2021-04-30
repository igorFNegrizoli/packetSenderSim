//#include "testbench.hpp"
#include "checksum16Bit.hpp"
#include <iostream>

int main(){

	bool errorFlag;
	uint16_t detectionFails = 0;

	for(uint64_t i=0; i<10000; ++i){
		checksum16Bit pkg(8);
		//pkg.printPacket('h');
		pkg.bernoulliModel(0.01);
		//pkg.printPacket('h');
		if(pkg.verifyChecksum()){
			++detectionFails;
			//pkg.printPacket('b');
		}
	}

	std::cout << std::endl << detectionFails << std::endl;

    return 0;
}