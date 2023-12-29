#include <iostream>
#include "Parser.h"

int main(int argc, char *argv[]){
    // std::cout << "Hi mama\n";
    std::string filename = argv[1];
    Parser parser(filename);
    parser.parseFile();
    parser.pCircuit_->topologicalSort();
    parser.pCircuit_->calculateKeyGateLocationCandidates();
    parser.pCircuit_->calculateAllOriginalWirePOs();
    parser.outputFilenameAppend_ = "_orig.v";
    if(argc > 2){
        std::string option(argv[2]);
        if(option == "-insertKeyGate"){
            parser.pCircuit_->insertKeyGates();
            parser.outputFilenameAppend_ = "_out.v";
        }
        else{
            std::cout << "Unknown argument\n";
        }
    }
    parser.writeVerilog();
    // parser.displayCircuitInfo();
    // for (Wire* pWire: parser.pCircuit_->keyGateLocationCandidates_){
    //     std::cout << "wire: " << pWire->name_ << " ";
    //     std::cout << "GATEs: ";
    //     for (Gate* pGate: pWire->POGatePtrs_)
    //     {
    //         std::cout << pGate->name_ << " ";
    //         std::cout << pGate->getTypeString() << " ";
    //     }
    //     std::cout << "\n";
    // }
}