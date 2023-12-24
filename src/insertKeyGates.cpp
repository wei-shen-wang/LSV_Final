#include "Circuit.h"

void Circuit::insertOneKeyGate(Wire* pWire){
    std::vector<Gate*> originalWireFanoutGatePtrs = pWire->outGatePtrs_;
    pWire->outGatePtrs_.clear();
    pWire->outputInserted_ = true;
    Wire* pKeyWire = new Wire(pWire->name_ + "_key");
    pKeyWire->inserted_ = true;
    Gate* pKeyGate = new Gate(pWire->name_ + "_key", Gate::XNOR);
    pKeyGate->inserted_ = true;
    pWire->outGatePtrs_.push_back(pKeyGate);
    pKeyGate->inWirePtrs_.push_back(pWire);
    pKeyGate->keyBitVal_ = 1;
    pKeyWire->inGatePtr_ = pKeyGate;
    pKeyGate->outWirePtrs_.push_back(pKeyWire);
    pKeyWire->outGatePtrs_ = originalWireFanoutGatePtrs;
    for (Gate* pGate : originalWireFanoutGatePtrs){
        for (int i = 0; i < pGate->inWirePtrs_.size(); ++i){
            if(pGate->inWirePtrs_[i] == pWire){
                pGate->inWirePtrs_[i] = pKeyWire;
            }
        }
    }
    if (this->outputNames_.find(pWire->name_) != this->outputNames_.end()){
        this->outputNames_.erase(pWire->name_);
        this->outputNames_.insert(pKeyWire->name_);
    }
    this->insertedKeyGatePtrs_.push_back(pKeyGate);
    this->insertedKeyWirePtrs_.push_back(pKeyWire);
}

void Circuit::insertInitialRandomKeyGates(){
    int numInitialKeyGates = this->getInitialNumKeyGates();
    for (int i = this->keyGateLocationCandidates_.size() - 1; i > 0; --i){
        Wire* pWire = this->keyGateLocationCandidates_[i];
        if (numInitialKeyGates == 0){
            break;
        }
        if (pWire->outputInserted_){
            continue;
        }
        this->insertOneKeyGate(pWire);
        --numInitialKeyGates;
        this->keyGateLocationCandidates_.pop_back();
    }
    // std::cerr << numInitialKeyGates << "\n";
    // std::cerr << this->keyGateLocationCandidates_.size() << "\n";
    // all xnor gates
    // calculate key gate insertion score
}

int Circuit::findKeyGateMetric(Gate* keyGatePtr, Wire* candidateKeyWirePtr){
    int metric = 0;
    assert(keyGatePtr->inWirePtrs_.size() == 1);
    Wire* pKeyWire = keyGatePtr->inWirePtrs_[0];
    // if isolated, metric = 0
    bool isolated = false;
    if (pKeyWire->POGatePtrs_.size() > candidateKeyWirePtr->POGatePtrs_.size()){
        for (Gate* pPOGate: pKeyWire->POGatePtrs_){
            if (candidateKeyWirePtr->POGatePtrs_.find(pPOGate) == candidateKeyWirePtr->POGatePtrs_.end()){
                isolated = true;
                break;
            }
        }
    }
    else{
        for (Gate* pPOGate: candidateKeyWirePtr->POGatePtrs_){
            if (pKeyWire->POGatePtrs_.find(pPOGate) == pKeyWire->POGatePtrs_.end()){
                isolated = true;
                break;
            }
        }
    }
    if (!isolated){
        metric = 1;
    }
    // mutable (consecutive, concurrent, sequential) metric = 1
    // non-mutable = 2
    return metric;
}

void Circuit::changeKeyVal(){
    // change key gate value
    // 50% change inserted xnor to xor + not
}

void Circuit::mergeKeyGate2OriginalCircuit(){
    // merge not gate to fanin gate(any)
}

void Circuit::calculateKeyGateLocationCandidates(){
    for (Wire* pWire: this->wirePtrs_){
        // if (this->inputNames_.find(pWire->name_) != this->inputNames_.end())
        // {
        //     continue;
        // }
        // if (this->outputNames_.find(pWire->name_) != this->outputNames_.end())
        // {
        //     continue;
        // }
        if(pWire->inGatePtr_->type_ == Gate::NOT){
            continue;
        }
        if(pWire->inGatePtr_->type_ == Gate::BUF){
            continue;
        }
        this->keyGateLocationCandidates_.push_back(pWire);
    }
}

void Circuit::calculateAllOriginalWirePOs(){
    for (Wire* pCandidateWire: this->keyGateLocationCandidates_){
        for (Gate* pGate: this->gatePtrs_){
            pGate->topoSorted_ = false;
        }
        for (Wire* pWire: this->wirePtrs_){
            pWire->topoSorted_ = false;
        }
        std::queue<Gate*> tempGatePtrs;
        for (Gate* pOutGate: pCandidateWire->outGatePtrs_){
            tempGatePtrs.push(pOutGate);
            pOutGate->topoSorted_ = true;
        }
        while (!tempGatePtrs.empty())
        {
            Gate* pGate = tempGatePtrs.front();
            tempGatePtrs.pop();
            if(pGate->type_ == Gate::PO){
                pCandidateWire->POGatePtrs_.insert(pGate);
                continue;
            }
            for (Wire* pWire: pGate->outWirePtrs_){
                if (!pWire->topoSorted_)
                {
                    pWire->topoSorted_ = true;
                    for (Gate* pOutGate: pWire->outGatePtrs_){
                        if(!pOutGate->topoSorted_){
                            tempGatePtrs.push(pOutGate);
                            pOutGate->topoSorted_ = true;
                        }
                    }
                }
            }
        }
    }
    
}

void Circuit::insertKeyGates(){
    // insert initial random key gates
    this->insertInitialRandomKeyGates();
    // while key gate number < 5% of total gate number
    bool firstTime = true;
    int num5PercentGates = this->get5PercentNumGates();
    Gate* pLastInsertedKeyGate = nullptr;
    int insertedIndex = 0;
    while (this->insertedKeyGatePtrs_.size() < num5PercentGates){
        if(this->keyGateLocationCandidates_.size() == 0){
            break;
        }
        if (firstTime){
            firstTime = false;
            // for each wire output not inserted with key gate
            for (Wire* pCandidatetWire: this->keyGateLocationCandidates_){
                // for each key gate
                pCandidatetWire->potentialKeyHardness_ = 0;
                for (Gate* pKeyGate: this->insertedKeyGatePtrs_){
                    // calculate key gate metric
                    pCandidatetWire->potentialKeyHardness_ += this->findKeyGateMetric(pKeyGate, pCandidatetWire);
                    // wire.potential key hardness += calculate key gate metric
                }
                // if (this->insertedKeyGatePtrs_.size() > pCandidatetWire->potentialKeyHardness_)
                //     std::cerr << "Max: " << this->insertedKeyGatePtrs_.size() << "\tHardeness: " << pCandidatetWire->potentialKeyHardness_ << "\n";
            }
        }
        else{
            // for each wire output not inserted with key gate
            for (Wire* pCandidatetWire: this->keyGateLocationCandidates_){
                // calculate key gate metric
                pCandidatetWire->potentialKeyHardness_ += this->findKeyGateMetric(pLastInsertedKeyGate, pCandidatetWire);
                // wire.potential key hardness += calculate key gate metric
                // if (this->insertedKeyGatePtrs_.size() > pCandidatetWire->potentialKeyHardness_)
                //     std::cerr << "Max: " << this->insertedKeyGatePtrs_.size() << "\tHardeness:" << pCandidatetWire->potentialKeyHardness_ << "\n";
            }
        }
        // choose wire with highest potential key hardness and insert xor (key gate)
        Wire* pWireWithHighestPotentialKeyHardness = nullptr;
        int highestPotentialKeyHardness = 0;
        int index = 0;
        for (Wire* pCandidatetWire: this->keyGateLocationCandidates_){
            if (pCandidatetWire->potentialKeyHardness_ > highestPotentialKeyHardness){
                highestPotentialKeyHardness = pCandidatetWire->potentialKeyHardness_;
                pWireWithHighestPotentialKeyHardness = pCandidatetWire;
                insertedIndex = index;
            }
            ++index;
        }
        if (pWireWithHighestPotentialKeyHardness == nullptr){
            break;
        }
        this->insertOneKeyGate(pWireWithHighestPotentialKeyHardness);
        pLastInsertedKeyGate = this->insertedKeyGatePtrs_.back();
        this->keyGateLocationCandidates_.erase(this->keyGateLocationCandidates_.begin() + insertedIndex);
    }
    // change key gate value
    // merge key gate to original circuit
}