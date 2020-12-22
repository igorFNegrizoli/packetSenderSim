#include "udp.h"

int main(){

	udpPacket pkg(8);
    pkg.printPacket('b');
    pkg.bernoulliModel(0.3);
    pkg.printPacket('b');
    return 0;
}