#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <map>
#include <set>
#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include "Gate.h"
class Circuit{
public:
    inline Circuit(){

    }
    inline ~Circuit(){
        for(int i = 0;i<int(gatePtrs_.size());++i){
            delete gatePtrs_[i];
        }
        for(int i = 0;i<int(wirePtrs_.size());++i){
            delete wirePtrs_[i];
        }
        for(int i = 0;i<int(insertedKeyGatePtrs_.size());++i){
            delete insertedKeyGatePtrs_[i];
        }
        for(int i = 0;i<int(insertedKeyWirePtrs_.size());++i){
            delete insertedKeyWirePtrs_[i];
        }
    }
    inline int get5PercentNumGates(){
        int ret = gatePtrs_.size()/20;
        if(ret <= getInitialNumKeyGates()){
            ret = getInitialNumKeyGates() + 1;
        }
        return ret;
    }
    inline int getInitialNumKeyGates(){
        int ret = gatePtrs_.size()/200;
        if(ret < 2){
            ret = 2;
        }
        return ret;
    }
    std::string name_;
    int keyGateInsertionScore_;
    std::vector<Gate*> insertedKeyGatePtrs_;
    std::vector<Wire*> insertedKeyWirePtrs_;
    std::vector<Wire*> keyGateLocationCandidates_; // key gate locations
    int findKeyGateMetric(Gate* keyGatePtr, Wire* candidateKeyWirePtr);
    void insertOneKeyGate(Wire* pWire);
    void insertInitialRandomKeyGates();
    void changeKeyVal();
    void insertKeyGates();
    void mergeKeyGate2OriginalCircuit();
    void calculateAllOriginalWirePOs();
    void calculateKeyGateLocationCandidates();

    void topologicalSort();

    std::vector<Gate*> gatePtrs_;
    std::vector<Wire*> wirePtrs_;
    std::map<std::string, Gate*> name2GatePtr_;
    std::map<std::string, Wire*> name2WirePtr_;
    std::map<Wire*, Wire*> wirePtr2wirePtr;
    std::set<std::string> inputNames_;
    std::set<std::string> outputNames_;
};
#endif