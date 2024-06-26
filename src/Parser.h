#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>
#include "Circuit.h"
class Parser
{
public:
    inline Parser(const std::string &filename){
        filename_ = filename;
        pCircuit_ = new Circuit();
        assignIndex_ = 0;
        tokenIndex_ = 0;
        wireIndex_ = 0;
        gateIndex_ = 0;
    }
    inline ~Parser(){
        delete pCircuit_;
    }
    void parseFile();
    void displayCircuitInfo();
    void writeVerilog();
    int tokenIndex_;
    int assignIndex_;
    int wireIndex_;
    int gateIndex_;
    std::vector<std::string> tokens_;
    Circuit* pCircuit_;
    std::string filename_;
    std::string outputFilenameAppend_;
};
#endif