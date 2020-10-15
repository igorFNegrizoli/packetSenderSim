#include <iostream>
#include <bitset>
#include "udp.h"
//#include "toolbox.h"

using namespace std;

int main(){
    uint16_t srcPort_ = 0xfff0, dstPort_ = 0x000f, length_ = 0x000c;
    uint16_t* data_ = new uint16_t(2);
    data_[0] = 0x0002;
    data_[1] = 0x0003;
    uint16_t* pack = buildUdpPacket(srcPort_, dstPort_, length_, data_);
    for(uint16_t i=0; i<length_/2; ++i){
    	cout << hex << pack[i] << endl;
    }
	injectErrorInPacket(pack, 1);
    return 0;
}
