#ifndef ADLER16BIT_HPP_INCLUDED
#define ADLER16BIT_HPP_INCLUDED
#include "FletcherAdler16Bit.hpp"

class Adler16Bit: public FletcherAdler16Bit {
public:
    Adler16Bit(){
        this->initialC1 = 0;
        this->initialC2 = 0;
        this->modulusVal = 251;
    }
    std::string getAlgName(){
        return "ADLR16";
    }
};

#endif
