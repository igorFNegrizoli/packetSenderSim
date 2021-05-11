#ifndef ERRORMODEL_HPP_INCLUDED
#define ERRORMODEL_HPP_INCLUDED
#include "Packet.hpp"
#include "RNG.hpp"

class ErrorModel {
   protected:
	RNG *rng; //random number generator
   public:
	ErrorModel();
	ErrorModel(RNG* rng_);
	
	/*
	  Retorna quantos bits foram invertidos
	*/
	virtual uint16_t injectErrors(Packet* packet_, bool forceError) = 0;
        
	RNG *getRNG();
	~ErrorModel();
};

#endif
