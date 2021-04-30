//#include "testbench.hpp"
#include "CRC16Bit.hpp"
#include <iostream>

int main(){
	
/*
	cout << "Executando testes" << endl;
	
	testBench testes;
	uint16_t count, nTestes = 10;
	for(uint16_t i=0; i<nTestes; ++i){
		//std::cout << i << " ";	
		count = testes.doTest();
		std::cout << count << ", ";	
	}

	std::cout << endl;
	
*/

	CRC16Bit pkg(8);
	pkg.printPacket('b');
	pkg.burstErrorPeriodicModel(2, 2, 80, 80);
	pkg.printPacket('b');
	std::cout << pkg.verifyChecksum() << std::endl;


    return 0;
}