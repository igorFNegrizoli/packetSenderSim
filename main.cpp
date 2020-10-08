#include <iostream>
#include <bitset>

using namespace std;

struct UdpPacket{
    uint16_t srcPort, dstPort, length, checksum;
    uint16_t* data;
}typedef UdpPacket;

UdpPacket* buildUdpPacket(uint16_t srcPort_ = 0, uint16_t dstPort_ = 0, uint16_t length_ = 8, uint16_t* data_ = nullptr){
    UdpPacket* pkg = new UdpPacket;
    uint32_t checksum_ = 0x00000000;
    pkg->srcPort = srcPort_;
    pkg->dstPort = dstPort_;
    if (0x00010000 & checksum_ != 0x00000000){
        ++checksum_;
        checksum_ = checksum_ & 0x0000ffff;
    }
    //implementar a verificação de soma do carry nos outros campos
    pkg->length = length_;
    checksum_ += pkg->srcPort;
    checksum_ += pkg->dstPort;
    checksum_ += pkg->length;
    if (data_ == nullptr) {
        pkg->checksum = checksum_;
        return pkg;
    }
    for(int i=0; i < length_ - 8; i++){
        checksum_ += data_[i];
    }
    pkg->checksum = checksum_;
    pkg->data = data_;
    return pkg; //retornar o C1 do checksum
}


int main(){
    /*
    uint16_t srcPort_ = 0x0001, dstPort_ = 0x0002, length_ = 0x000a;
    uint16_t* data_ = new uint16_t(2);
    data_[0] = 0x00b0;
    data_[1] = 0x0010;
    UdpPacket* pack = buildUdpPacket(srcPort_, dstPort_, length_, data_);
    cout << hex << pack->srcPort << endl << pack->dstPort << endl << pack->length << endl << pack->data[0] << endl << pack->data[1] << endl << pack->checksum;
    */
   uint32_t check = 
    return 0;
}