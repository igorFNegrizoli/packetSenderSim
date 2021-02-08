#include "testbench.h"
#include <iostream>
#include "udp.h"

using namespace std;

int main(){

	testBench testes;
	uint16_t count;
	count = testes.doTest();
	cout << count << endl;
	/*
	udpPacket pkg(8);
	pkg.printPacket('h');
	//pkg.bernoulliModel(0.1);
	pkg.printPacket('h');
	cout << pkg.verifyChecksum() << endl;
	*/
    return 0;
}