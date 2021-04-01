#include "testbench.hpp"
#include <iostream>


using namespace std;

int main(){

	cout << "Executando testes" << endl;
	
	testBench testes;
	uint16_t count, nTestes = 10;
	for(uint16_t i=0; i<nTestes; ++i){
		count = testes.doTest();
		cout << count << ", ";	
	}

	cout << endl;
	

	/*
	udpPacket pkg(8);
	pkg.printPacket('b');
	pkg.burstErrorPeriodicModel(8, 8, 2, 2);
	pkg.printPacket('b');
	cout << pkg.verifyChecksum() << endl;
	*/

    return 0;
}