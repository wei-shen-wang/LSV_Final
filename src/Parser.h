#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "Circuit.h"
class Parser
{
public:
    inline Parser(const std::string &filename){
        filename_ = filename;
        pCircuit_ = new Circuit();
    }
    inline ~Parser(){
        delete pCircuit_;
    }
    int parseFile();
    // void parseLine(const std::string &line);
    int tokenIndex_;
    std::vector<std::string> tokens_;
    Circuit* pCircuit_;
    std::string filename_;

};