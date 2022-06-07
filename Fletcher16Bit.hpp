#ifndef FLETCHER16BIT_HPP_INCLUDED
#define FLETCHER16BIT_HPP_INCLUDED
#include "FletcherAdler16Bit.hpp"

class Fletcher16Bit: public FletcherAdler16Bit {
public:
    Fletcher16Bit(bool is1sComp){
        this->initialC1 = 0;
        this->initialC2 = 0;

        if(is1sComp){
            this->modulusVal = 255;
        }else{
            this->modulusVal = 256;
        }
    }
    std::string getAlgName(){
        if(this->modulusVal == 255){
            return "1CFL16";
        }else{
            return "2CFL16";
        }
    }
};

#endif
