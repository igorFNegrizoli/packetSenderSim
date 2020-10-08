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
    pkg->length = length_;
    checksum_ += pkg->srcPort;
    checksum_ += pkg->dstPort;
    if ((0x11110000 & checksum_) != 0x00000000){
        ++checksum_;
        checksum_ = checksum_ & 0x0000ffff;
    }
    checksum_ += pkg->length;
    if ((0x11110000 & checksum_) != 0x00000000){
            cout << "flag" << hex << checksum_ << endl;
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
    }
    if (data_ == nullptr) {
        pkg->checksum = checksum_;
        return pkg;
    }
    for(int i=0; i < length_ - 8; i++){
        checksum_ += data_[i];
        if ((0x11110000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
        }
    }
    pkg->checksum = checksum_;
    pkg->data = data_;
    return ~pkg;
}


int main(){
    //testar retorno do complemento de 1
    uint16_t srcPort_ = 0xfff0, dstPort_ = 0x000f, length_ = 0x000a;
    uint16_t* data_ = new uint16_t(2);
    data_[0] = 0x002;
    data_[1] = 0x0003;
    UdpPacket* pack = buildUdpPacket(srcPort_, dstPort_, length_, data_);
    cout << hex << pack->srcPort << endl << pack->dstPort << endl << pack->length << endl << pack->data[0] << endl << pack->data[1] << endl << pack->checksum << endl;

    return 0;
}
