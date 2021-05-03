#include "CRC16Bit.hpp"
#include "checksum16Bit.hpp"
#include <iostream>

int main(){
/*
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
*/

	checksum16Bit* pkg = new checksum16Bit(10);
	pkg->printPacket('h');
	CRC16Bit* pkg2 = new CRC16Bit(10);
	std::cout << std::endl;
	pkg2->printPacket('h');
	
	uint16_t* teste = pkg->getData();
	for(int i=0; i<4; i++) std::cout << teste[i] << " " ;
	std::cout << std::endl;
	teste = pkg2->getData();
	for(int i=0; i<4; i++) std::cout << teste[i] << " " ;
	std::cout << std::endl;
		
    return 0;
}