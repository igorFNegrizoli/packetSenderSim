#include "bitErrorModels.h"
#include "udp.h"
#include <iostream>

using namespace std;

uint16_t* constructRandomPacket(uint16_t len){
	uint16_t src = randomIntInterval(0, 0xffff);
	uint16_t dst = randomIntInterval(0, 0xffff);
	uint16_t* data = new uint16_t((len-8)/2);
	for(uint16_t i=0; i<((len-8)/2); ++i){
		data[i] = randomIntInterval(0, 0xffff);
		cout << hex <<data[i] << " " << endl;
	}
	uint16_t* packet = buildUdpPacket(src, dst, len, data);
	for(uint16_t i=0; i<(len/2); ++i){
		cout << hex <<packet[i] << " ";
	}
	return packet;
}
uint16_t pureRandomModel_Test(uint16_t lowestErrQ = 1, uint16_t highestErrQ = 10, uint16_t lowestPcktLength = 0xa, uint16_t highestPcktLength = 0xffff, uint16_t nIter = 50){
	uint16_t auxLen, goodCount = 0, badCount = 0;
	bool flag;
	uint16_t* p1=nullptr;
	for (uint16_t i=lowestErrQ; i<=highestErrQ; ++i){
		for(uint16_t j=lowestPcktLength; j<=highestPcktLength; j+=2){
			for(uint16_t k=0; k<=nIter; ++k){
				p1 = constructRandomPacket(j);
				auxLen = p1[2];
				p1 = pureRandomModel(p1, i);
				flag = receiveUdpPacket(p1, auxLen);
				if(flag == true) ++goodCount; else ++badCount;
			}
		}
	}
	cout << "pureRandomModel:  " << "\n\tBad/Good: " << badCount << " / " << goodCount;
	return badCount;
}