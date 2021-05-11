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
