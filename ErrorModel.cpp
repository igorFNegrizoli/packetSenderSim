//Funcionamento dos algoritmos das implementações desta interface estão descritos em 
//How the algorithms written on the implementations of this interface are described at 
//http://www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf

#include "ErrorModel.hpp"

ErrorModel::ErrorModel(){
	this->rng = nullptr;
}

ErrorModel::ErrorModel(RNG* rng_) {	
        this->rng = rng_;
}

RNG *ErrorModel::getRNG() {
	return this->rng;
}

ErrorModel::~ErrorModel(){
}
