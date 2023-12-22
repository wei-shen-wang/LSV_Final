#include "Circuit.h"

void Circuit::topologicalSort(){
    std::queue<Gate*> tempGatePtrs;
    std::vector<Gate*> topoOrderGatePtrs;
    std::vector<Wire*> topoOrderWirePtrs;
    for (int i = 0; i < this->gatePtrs_.size(); ++i){
        Gate* pGate = this->gatePtrs_[i];
        if (pGate->type_ == Gate::PI){
            topoOrderGatePtrs.push_back(pGate);
            tempGatePtrs.push(pGate);
            pGate->topoSorted_ = true;
        }
    }
    while (!tempGatePtrs.empty())
    {
        Gate* pGate = tempGatePtrs.front();
        tempGatePtrs.pop();
        for (int i = 0; i < pGate->outWirePtrs_.size(); ++i){
            Wire* pWire = pGate->outWirePtrs_[i];
            if(!pWire->topoSorted_){
                topoOrderWirePtrs.push_back(pWire);
                pWire->topoSorted_ = true;
            }
            for (int j = 0; j < pWire->outGatePtrs_.size(); ++j){
                Gate* pOutGate = pWire->outGatePtrs_[j];
                tempGatePtrs.push(pOutGate);
                if(!pOutGate->topoSorted_){
                    topoOrderGatePtrs.push_back(pOutGate);
                    pOutGate->topoSorted_ = true;
                }
            }
        }
    }
    
    std::cerr << "gate count: " << this->gatePtrs_.size() << "\n";
    std::cerr << "topoOrderGatePtrs.size: " << topoOrderGatePtrs.size() << "\n";
    std::cerr << "wire count: " << this->wirePtrs_.size() << "\n";
    std::cerr << "topoOrderWirePtrs.size: " << topoOrderWirePtrs.size() << "\n";
    // assert(topoOrderGatePtrs.size() == this->gatePtrs_.size());
    // assert(topoOrderWirePtrs.size() == this->wirePtrs_.size());
    this->gatePtrs_ = topoOrderGatePtrs;
    this->wirePtrs_ = topoOrderWirePtrs;
    
}