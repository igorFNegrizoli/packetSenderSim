#include "testbench.h"
#include <iostream>

using namespace std;

int main(){

	testBench testes;
	uint16_t count;
	count = testes.doTest();
	cout << count << endl;
    return 0;
}