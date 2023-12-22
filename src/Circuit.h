#include <unordered_map>
#include <unordered_set>
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
        for(int i = 0;i<gatePtrs_.size();++i){
            delete gatePtrs_[i];
        }
        for(int i = 0;i<wirePtrs_.size();++i){
            delete wirePtrs_[i];
        }
    }
    void topologicalSort();
    std::vector<Gate*> gatePtrs_;
    std::vector<Wire*> wirePtrs_;
    std::unordered_map<std::string, Gate*> name2GatePtr_;
    std::unordered_map<std::string, Wire*> name2WirePtr_;
    std::unordered_map<Wire*, Wire*> wirePtr2wirePtr;
    std::unordered_set<std::string> inputNames_;
    std::unordered_set<std::string> outputNames_;
};