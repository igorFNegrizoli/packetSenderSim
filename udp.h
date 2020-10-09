struct UdpPacket{
    uint16_t srcPort, dstPort, length, checksum;
    uint16_t* data;
}typedef UdpPacket;

uint16_t doChecksum(uint16_t srcPort_ = 0, uint16_t dstPort_ = 0, uint16_t length_ = 8, uint16_t* data_ = nullptr){

    uint32_t checksum_ = 0x00000000;
    uint32_t checksum = 0;
    checksum_ += srcPort_;
    checksum_ += dstPort_;
    if ((0xffff0000 & checksum_) != 0x00000000){
        ++checksum_;
        checksum_ = checksum_ & 0x0000ffff;
    }
    checksum_ += length_;
    if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
    }
    if (data_ == nullptr) {
        checksum = checksum_;
        return checksum;
    }
    for(int i=0; i < (length_ - 8)/2; i++){
        checksum_ += data_[i];
        if ((0xffff0000 & checksum_) != 0x00000000){
            ++checksum_;
            checksum_ = checksum_ & 0x0000ffff;
        }
    }
    checksum = checksum_;
    return checksum;
}

UdpPacket* buildUdpPacket(uint16_t srcPort_ = 0, uint16_t dstPort_ = 0, uint16_t length_ = 8, uint16_t* data_ = nullptr){
    UdpPacket* pkg = new UdpPacket;
    pkg->srcPort = srcPort_;
    pkg->dstPort = dstPort_;
    pkg->length = length_;
    pkg->checksum = ~doChecksum(srcPort_, dstPort_, length_, data_);
    pkg->data = data_;
    return pkg;
}

bool receiveUdpPacket(UdpPacket* packet=nullptr){
    uint16_t chc;
    chc = doChecksum(packet->srcPort, packet->dstPort, packet->length, packet->data);
    if(chc + packet->checksum != 0xffff){
        return false;
    }
    return true;
}


//UdpPacket* injectErrorInPacket(UdpPacket* pkg, uint16_t errorQuantity = 1){


//mudar estrutura? pesquisar sobre vetores estatico em structs