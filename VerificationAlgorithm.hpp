#ifndef VERIFICATIONALGORITHM_HPP_INCLUDED
#define VERIFICATIONALGORITHM_HPP_INCLUDED
#include "Packet.hpp"

class VerificationAlgorithm {
   public:
	VerificationAlgorithm(){}
	virtual uint32_t generateVerificationCode(Packet* packet_) = 0;
	~VerificationAlgorithm(){}
};

#endif