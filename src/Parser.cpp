#include "Parser.h"

void Parser::parseFile(){
    std::ifstream inFile(this->filename_, std::ifstream::in);
    if (!inFile) {
        std::cerr << "cannot open input file\n";
        exit(1);
    }
    std::string word;
    std::stringstream sstream;
    // int lineNumber;
    bool verilogEndline = false;
    int maxFanin = 0;
    while (!inFile.eof() && !inFile.bad()) {
        word.clear();
        inFile >> word;
        if(word[0] == '/'){
            getline(inFile, word);
            continue;
        }

        if(word.find('(') != std::string::npos){
            if (this->tokens_[0] == "module") {
                word.erase(word.find('('), word.size() - word.find('('));
            }
            else{
                word.erase(0, word.find('(')+1);
            }
        }
        if(word.find(')') != std::string::npos){
            word.erase(word.find(')'),1);
        }
        if(word.find(',') != std::string::npos){
            word.erase(word.find(','),1);
        }
        if(word.find(';') != std::string::npos){
            word.erase(word.find(';'),1);
            verilogEndline = true;
        }
        if(word != ""){
            this->tokens_.push_back(word);
        }
        if(verilogEndline){
            // for(int i = 0;i<this->tokens_.size();++i){
            //     std::cerr << this->tokens_[i] << " ";
            // }
            // std::cerr << "\n";
            if(this->tokens_[0] == "module"){
                this->pCircuit_->name_ = this->tokens_[1];
                std::cerr << "module name: " << this->pCircuit_->name_ << "\n";
            }
            else if(this->tokens_[0] == "input"){
                for (int i = 1; i < int(this->tokens_.size()); ++i) {
                    std::string inputName = this->tokens_[i];
                    Gate* pGate = new Gate(inputName, Gate::PI);
                    pGate->index_ = this->gateIndex_;
                    this->pCircuit_->gatePtrs_.push_back(pGate);
                    this->pCircuit_->name2GatePtr_[inputName] = pGate;
                    this->pCircuit_->inputNames_.insert(inputName);
                }
                ++this->gateIndex_;
            }
            else if(this->tokens_[0] == "output"){
                for (int i = 1; i < int(this->tokens_.size()); ++i) {
                    std::string outputName = this->tokens_[i];
                    Gate* pGate = new Gate(outputName, Gate::PO);
                    pGate->index_ = this->gateIndex_;
                    this->pCircuit_->gatePtrs_.push_back(pGate);
                    this->pCircuit_->name2GatePtr_[outputName] = pGate;
                    this->pCircuit_->outputNames_.insert(outputName);
                }
                ++this->gateIndex_;
            }
            else if(this->tokens_[0] == "wire"){
                for (int i = 1; i < int(this->tokens_.size()); ++i) {
                    std::string wireName = this->tokens_[i];
                    Wire* pWire = new Wire(wireName);
                    pWire->name_ = wireName;
                    pWire->index_ = this->wireIndex_;
                    if(this->pCircuit_->inputNames_.find(wireName) != this->pCircuit_->inputNames_.end()){
                        pWire->inGatePtr_ = this->pCircuit_->name2GatePtr_[wireName];
                        this->pCircuit_->name2GatePtr_[wireName]->outWirePtrs_.push_back(pWire);
                    }
                    if (this->pCircuit_->outputNames_.find(wireName) != this->pCircuit_->outputNames_.end()) {
                        pWire->outGatePtrs_.push_back(this->pCircuit_->name2GatePtr_[wireName]);
                        this->pCircuit_->name2GatePtr_[wireName]->inWirePtrs_.push_back(pWire);
                    }
                    this->pCircuit_->wirePtrs_.push_back(pWire);
                    this->pCircuit_->name2WirePtr_[wireName] = pWire;
                }
                ++this->wireIndex_;
            }
            else if(this->tokens_[0] == "assign"){
                // treat as buffer
                std::string gateName = this->tokens_[0] + std::to_string(this->assignIndex_);
                Gate* pGate = new Gate(gateName, Gate::BUF);
                pGate->index_ = this->gateIndex_;
                pGate->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[3]]);
                pGate->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[1]]);
                this->pCircuit_->name2WirePtr_[this->tokens_[3]]->outGatePtrs_.push_back(pGate);
                this->pCircuit_->name2WirePtr_[this->tokens_[1]]->inGatePtr_ = pGate;
                this->pCircuit_->gatePtrs_.push_back(pGate);
                this->pCircuit_->name2GatePtr_[gateName] = pGate;
                ++this->assignIndex_;
                ++this->gateIndex_;
            }
            else if(this->tokens_[0] == "not" || this->tokens_[0] == "buf"){
                Gate::Type type = this->tokens_[0] == "not" ? Gate::NOT : Gate::BUF;
                std::string gateName = this->tokens_[1];
                Gate* pGate = new Gate(gateName, type);
                pGate->index_ = this->gateIndex_;
                pGate->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[3]]);
                pGate->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                this->pCircuit_->name2WirePtr_[this->tokens_[3]]->outGatePtrs_.push_back(pGate);
                this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtr_ = pGate;
                this->pCircuit_->gatePtrs_.push_back(pGate);
                this->pCircuit_->name2GatePtr_[gateName] = pGate;
                ++this->gateIndex_;
            }
            else if(this->tokens_[0] == "nand" || this->tokens_[0] == "and"
             || this->tokens_[0] == "nor" || this->tokens_[0] == "or"
             || this->tokens_[0] == "xor" || this->tokens_[0] == "xnor"){
                if(this->tokens_.size() == 5){ // 2-input
                    Gate::Type type;
                    if(this->tokens_[0] == "nand"){
                        type = Gate::NAND;
                    }
                    else if(this->tokens_[0] == "and"){
                        type = Gate::AND;
                    }
                    else if(this->tokens_[0] == "nor"){
                        type = Gate::NOR;
                    }
                    else if(this->tokens_[0] == "or"){
                        type = Gate::OR;
                    }
                    else if(this->tokens_[0] == "xor"){
                        type = Gate::XOR;
                    }
                    else if(this->tokens_[0] == "xnor"){
                        type = Gate::XNOR;
                    }
                    std::string gateName = this->tokens_[1];
                    Gate* pGate = new Gate(gateName, type);
                    pGate->index_ = this->gateIndex_;
                    for (int i = 3; i < int(this->tokens_.size()); ++i) { // 2-input->size=5
                        pGate->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[i]]);
                        this->pCircuit_->name2WirePtr_[this->tokens_[i]]->outGatePtrs_.push_back(pGate);
                    }
                    pGate->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                    this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtr_ = pGate;
                    this->pCircuit_->gatePtrs_.push_back(pGate);
                    this->pCircuit_->name2GatePtr_[gateName] = pGate;
                    ++this->gateIndex_;
                    // if(this->tokens_.size()>5){
                    //     std::cerr << this->tokens_[0] << " " << this->tokens_.size()-3 << std::endl;
                    // }
                }
                else if(this->tokens_.size() == 6){
                    Gate::Type type1, type2;
                    if(this->tokens_[0] == "nand" || this->tokens_[0] == "and"){
                        type1 = Gate::NOR;
                        type2 = Gate::NAND;
                    }
                    else{ // nor/or
                        type1 = Gate::NAND;
                        type2 = Gate::NOR;
                    }
                    std::string gateName = this->tokens_[1];
                    std::string wireName1 = gateName + "_inw1";
                    Wire* pWire_in1 = new Wire(wireName1);
                    pWire_in1->name_ = wireName1;
                    pWire_in1->index_ = this->pCircuit_->wirePtrs_.size();
                    this->pCircuit_->wirePtrs_.push_back(pWire_in1);
                    this->pCircuit_->name2WirePtr_[wireName1] = pWire_in1;
                   
                    Gate* pGate = new Gate(gateName, type1);
                    pGate->index_ = this->gateIndex_;
                    pGate->inWirePtrs_.push_back(pWire_in1);
                    pGate->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[5]]);
                    pWire_in1->outGatePtrs_.push_back(pGate);
                    this->pCircuit_->name2WirePtr_[this->tokens_[5]]->outGatePtrs_.push_back(pGate);
                    this->pCircuit_->gatePtrs_.push_back(pGate);
                    this->pCircuit_->name2GatePtr_[gateName] = pGate;
                    ++this->gateIndex_;

                    std::string gateName1 = gateName + "_ing1";
                    Gate* pGate_in1 = new Gate(gateName1, type2);
                    pGate_in1->index_ = this->gateIndex_;
                    pGate_in1->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[3]]);
                    pGate_in1->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[4]]);
                    this->pCircuit_->name2WirePtr_[this->tokens_[3]]->outGatePtrs_.push_back(pGate_in1);
                    this->pCircuit_->name2WirePtr_[this->tokens_[4]]->outGatePtrs_.push_back(pGate_in1);
                    this->pCircuit_->gatePtrs_.push_back(pGate_in1);
                    this->pCircuit_->name2GatePtr_[gateName1] = pGate_in1;
                    pGate_in1->outWirePtrs_.push_back(pWire_in1);
                    pWire_in1->inGatePtr_ = pGate_in1;
                    ++this->gateIndex_;

                    if(this->tokens_[0] == "and" || this->tokens_[0] == "or"){
                        // and/or
                        pGate->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                        this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtr_ = pGate;
                        // this->pCircuit_->gatePtrs_.push_back(pGate);
                        // this->pCircuit_->name2GatePtr_[gateName] = pGate;
                    }
                    else{
                        std::string wireNameOut = gateName + "_invw";
                        Wire* pWire_out = new Wire(wireNameOut);
                        pWire_out->name_ = wireNameOut;
                        pWire_out->index_ = this->pCircuit_->wirePtrs_.size();
                        this->pCircuit_->wirePtrs_.push_back(pWire_out);
                        this->pCircuit_->name2WirePtr_[wireNameOut] = pWire_out;
                        pGate->outWirePtrs_.push_back(pWire_out);
                        pWire_out->inGatePtr_ = pGate;
                        std::string gateNameOut = gateName + "_outg";
                        Gate* pGate_inv = new Gate(gateNameOut, Gate::NOT);
                        pGate_inv->index_ = this->gateIndex_;
                        pGate_inv->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                        this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtr_ = pGate_inv;
                        this->pCircuit_->gatePtrs_.push_back(pGate_inv);
                        pGate_inv->inWirePtrs_.push_back(pWire_out);
                        pWire_out->inGatePtr_ = pGate_inv;
                        pWire_out->outGatePtrs_.push_back(pGate_inv);
                        ++this->gateIndex_;
                    }
                }
                else if(this->tokens_.size() == 7){
                    Gate::Type type1, type2;
                    if(this->tokens_[0] == "nand" || this->tokens_[0] == "and"){
                        type1 = Gate::NOR;
                        type2 = Gate::NAND;
                    }
                    else{ // nor/or
                        type1 = Gate::NAND;
                        type2 = Gate::NOR;
                    }
                    std::string gateName = this->tokens_[1];
                    std::string wireName1 = gateName + "_inw1";
                    Wire* pWire_in1 = new Wire(wireName1);
                    pWire_in1->name_ = wireName1;
                    pWire_in1->index_ = this->pCircuit_->wirePtrs_.size();
                    this->pCircuit_->wirePtrs_.push_back(pWire_in1);
                    this->pCircuit_->name2WirePtr_[wireName1] = pWire_in1;
                    std::string wireName2 = gateName + "_inw2";
                    Wire* pWire_in2 = new Wire(wireName2);
                    pWire_in2->name_ = wireName2;
                    pWire_in2->index_ = this->pCircuit_->wirePtrs_.size();
                    this->pCircuit_->wirePtrs_.push_back(pWire_in2);
                    this->pCircuit_->name2WirePtr_[wireName2] = pWire_in2;
                   
                    Gate* pGate = new Gate(gateName, type1);
                    pGate->index_ = this->gateIndex_;
                    pGate->inWirePtrs_.push_back(pWire_in1);
                    pGate->inWirePtrs_.push_back(pWire_in2);
                    pWire_in1->outGatePtrs_.push_back(pGate);
                    pWire_in2->outGatePtrs_.push_back(pGate);
                    this->pCircuit_->gatePtrs_.push_back(pGate);
                    this->pCircuit_->name2GatePtr_[gateName] = pGate;
                    ++this->gateIndex_;

                    std::string gateName1 = gateName + "_ing1";
                    Gate* pGate_in1 = new Gate(gateName1, type2);
                    pGate_in1->index_ = this->gateIndex_;
                    pGate_in1->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[3]]);
                    pGate_in1->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[4]]);
                    this->pCircuit_->name2WirePtr_[this->tokens_[3]]->outGatePtrs_.push_back(pGate_in1);
                    this->pCircuit_->name2WirePtr_[this->tokens_[4]]->outGatePtrs_.push_back(pGate_in1);
                    pGate_in1->outWirePtrs_.push_back(pWire_in1);
                    pWire_in1->inGatePtr_ = pGate_in1;
                    this->pCircuit_->gatePtrs_.push_back(pGate_in1);
                    this->pCircuit_->name2GatePtr_[gateName1] = pGate_in1;
                    ++this->gateIndex_;

                    std::string gateName2 = gateName + "_ing2";
                    Gate* pGate_in2 = new Gate(gateName2, type2);
                    pGate_in2->index_ = this->gateIndex_;
                    pGate_in2->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[5]]);
                    pGate_in2->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[6]]);
                    this->pCircuit_->name2WirePtr_[this->tokens_[5]]->outGatePtrs_.push_back(pGate_in2);
                    this->pCircuit_->name2WirePtr_[this->tokens_[6]]->outGatePtrs_.push_back(pGate_in2);
                    pGate_in2->outWirePtrs_.push_back(pWire_in2);
                    pWire_in2->inGatePtr_ = pGate_in2;
                    this->pCircuit_->gatePtrs_.push_back(pGate_in2);
                    this->pCircuit_->name2GatePtr_[gateName2] = pGate_in2;
                    ++this->gateIndex_;

                    if(this->tokens_[0] == "and" || this->tokens_[0] == "or"){
                        // and/or
                        pGate->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                        this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtr_ = pGate;
                        // this->pCircuit_->gatePtrs_.push_back(pGate);
                        // this->pCircuit_->name2GatePtr_[gateName] = pGate;
                    }
                    else{
                        std::string wireNameOut = gateName + "_invw";
                        Wire* pWire_out = new Wire(wireNameOut);
                        pWire_out->name_ = wireNameOut;
                        pWire_out->index_ = this->pCircuit_->wirePtrs_.size();
                        this->pCircuit_->wirePtrs_.push_back(pWire_out);
                        this->pCircuit_->name2WirePtr_[wireNameOut] = pWire_out;
                        pWire_out->inGatePtr_ = pGate;
                        pGate->outWirePtrs_.push_back(pWire_out);
                        std::string gateNameOut = gateName + "_outg";
                        Gate* pGate_inv = new Gate(gateNameOut, Gate::NOT);
                        pGate_inv->index_ = this->gateIndex_;
                        pGate_inv->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                        this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtr_ = pGate_inv;
                        this->pCircuit_->gatePtrs_.push_back(pGate_inv);
                        pGate_inv->inWirePtrs_.push_back(pWire_out);
                        pWire_out->inGatePtr_ = pGate_inv;
                        pWire_out->outGatePtrs_.push_back(pGate_inv);
                        ++this->gateIndex_;
                    }
                }
                else {
                    std::cerr << "Too many input QAQ\n";
                }
            }
            else {
                std::cerr << "Wang: RAHHHHHHHHHHHH\n";
            }
            this->tokens_.clear();
            verilogEndline = false;
        }
    }
    // std::cerr << "Start checking 2-input gates splitting\n";
    // // check
    // for(Gate* pGate : this->pCircuit_->gatePtrs_){
    //     if(pGate->type_ == Gate::PI || pGate->type_ == Gate::PO || pGate->type_ == Gate::BUF || pGate->type_ == Gate::NOT) continue;
    //     if(pGate->inWirePtrs_.size() != 2 || pGate->outWirePtrs_.size() != 1){
    //         std::cerr << "ERROR!!\n";
    //         std::cerr << "Gate " << pGate->name_ << " type: " << pGate->getTypeString() << "\n";
    //         std::cerr << "\tinWirePtrs_: ";
    //         for(Wire* pWire : pGate->inWirePtrs_){
    //             std::cerr << pWire->name_ << " ";
    //         }
    //         std::cerr << "\n";
    //         std::cerr << "\toutWirePtrs_: ";
    //         for(Wire* pWire : pGate->outWirePtrs_){
    //             std::cerr << pWire->name_ << " ";
    //         }
    //         std::cerr << "\n";
    //     }
    // }
    // for(Wire* pWire : this->pCircuit_->wirePtrs_){
    //     if(pWire->inGatePtr_ == nullptr || pWire->outGatePtrs_.size() == 0){
    //         std::cerr << "ERROR!!\n";
    //         std::cerr << "Wire " << pWire->name_ << "\n";
    //         std::cerr << "\tinGatePtr_: ";
    //         std::cerr << pWire->inGatePtr_->name_ << " ";
    //         std::cerr << "\n";
    //         std::cerr << "\toutGatePtrs_: ";
    //         for(Gate* pGate : pWire->outGatePtrs_){
    //             std::cerr << pGate->name_ << " ";
    //         }
    //         std::cerr << "\n";
    //     }
    // }
    // std::cerr << "Finish checking...\n";
    
}

void Parser::displayCircuitInfo(){
    // std::cerr << "inputNames_: \n";
    // for (std::string inputName : this->pCircuit_->inputNames_) {
    //     std::cerr << "Gate " << this->pCircuit_->name2GatePtr_[inputName]->name_ << " type: " << this->pCircuit_->name2GatePtr_[inputName]->getTypeString() << "\n";
    // }
    // std::cerr << "outputNames_: \n";
    // for (std::string outputName : this->pCircuit_->outputNames_) {
    //     std::cerr << "Gate " << this->pCircuit_->name2GatePtr_[outputName]->name_ << " type: " << this->pCircuit_->name2GatePtr_[outputName]->getTypeString() << "\n";
    // }
    std::cerr << "\n";
    std::cerr << "Gate info:\n";
    for(Gate* pGate : this->pCircuit_->gatePtrs_){
        std::cerr << "Gate " << pGate->name_ << " type: " << pGate->getTypeString() << "\n";
        std::cerr << "\tinWirePtrs_: ";
        for(Wire* pWire : pGate->inWirePtrs_){
            std::cerr << pWire->name_ << " ";
        }
        std::cerr << "\n";
        std::cerr << "\toutWirePtrs_: ";
        for(Wire* pWire : pGate->outWirePtrs_){
            std::cerr << pWire->name_ << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "Wire info:\n";
    for(Wire* pWire : this->pCircuit_->wirePtrs_){
        std::cerr << "Wire " << pWire->name_ << "\n";
        std::cerr << "\tinGatePtr_: ";
        std::cerr << pWire->inGatePtr_->name_ << " ";
        std::cerr << "\n";
        std::cerr << "\toutGatePtrs_: ";
        for(Gate* pGate : pWire->outGatePtrs_){
            std::cerr << pGate->name_ << " ";
        }
        std::cerr << "\n";
    }
}