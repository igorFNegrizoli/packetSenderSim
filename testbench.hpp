#ifndef TESTBENCH_HPP_INCLUDED
#define TESTBENCH_HPP_INCLUDED
#include <cstdint>
#include <string>

using namespace std;


/*
Classe feita para realizar os testes de eficiencia. Futuramente essa mesma classe sera capaz de 
trabalhar com os demais protocolos
Class made with the purpose of executing the efficiency tests. In the future this class will be 
able to work with other protocols
*/

class testBench{
        uint16_t detectionFails, totalPackets, len;
        string protocol;
    public:
        testBench();
        testBench(string protocol_, uint16_t totalPackets_, uint16_t len);
        uint16_t doTest();
        ~testBench();
};

#endif
