#include "Parser.h"

int Parser::parseFile(){
    std::ifstream inFile(this->filename_, std::ifstream::in);
    if (!inFile) {
        std::cerr << "cannot open input file\n";
        exit(1);
    }
    std::string word;
    std::stringstream sstream;
    int lineNumber;
    bool verilogEndline = false;
    while (!inFile.eof() && !inFile.bad()) {
        word.clear();
        inFile >> word;
        if(word[0] == '/'){
            getline(inFile, word);
            continue;
        }

        if(word.find('(') != std::string::npos){
            word.erase(0, word.find('(')+1);
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
                // do nothing
            }
            else if(this->tokens_[0] == "input"){
                for (int i = 1; i < this->tokens_.size(); ++i) {
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
                for (int i = 1; i < this->tokens_.size(); ++i) {
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
                for (int i = 1; i < this->tokens_.size(); ++i) {
                    std::string wireName = this->tokens_[i];
                    Wire* pWire = new Wire(wireName);
                    pWire->name_ = wireName;
                    pWire->index_ = this->wireIndex_;
                    if(this->pCircuit_->inputNames_.find(wireName) != this->pCircuit_->inputNames_.end()){
                        pWire->inGatePtrs_.push_back(this->pCircuit_->name2GatePtr_[wireName]);
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
                this->pCircuit_->name2WirePtr_[this->tokens_[1]]->inGatePtrs_.push_back(pGate);
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
                this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtrs_.push_back(pGate);
                this->pCircuit_->gatePtrs_.push_back(pGate);
                this->pCircuit_->name2GatePtr_[gateName] = pGate;
                ++this->gateIndex_;
            }
            else if(this->tokens_[0] == "nand" || this->tokens_[0] == "and"
             || this->tokens_[0] == "nor" || this->tokens_[0] == "or"
             || this->tokens_[0] == "xor" || this->tokens_[0] == "xnor"){
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
                for (int i = 3; i < this->tokens_.size(); ++i) {
                    pGate->inWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[i]]);
                    this->pCircuit_->name2WirePtr_[this->tokens_[i]]->outGatePtrs_.push_back(pGate);
                }
                pGate->outWirePtrs_.push_back(this->pCircuit_->name2WirePtr_[this->tokens_[2]]);
                this->pCircuit_->name2WirePtr_[this->tokens_[2]]->inGatePtrs_.push_back(pGate);
                this->pCircuit_->gatePtrs_.push_back(pGate);
                this->pCircuit_->name2GatePtr_[gateName] = pGate;
                ++this->gateIndex_;
            }
            this->tokens_.clear();
            verilogEndline = false;
        }
    }
    
}

int Parser::displayCircuitInfo(){
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
    for(int i = 0;i<this->pCircuit_->gatePtrs_.size();++i){
        std::cerr << "Gate " << this->pCircuit_->gatePtrs_[i]->name_ << " type: " << this->pCircuit_->gatePtrs_[i]->getTypeString() << "\n";
        std::cerr << "\tinWirePtrs_: ";
        for(int j = 0;j<this->pCircuit_->gatePtrs_[i]->inWirePtrs_.size();++j){
            std::cerr << this->pCircuit_->gatePtrs_[i]->inWirePtrs_[j]->name_ << " ";
        }
        std::cerr << "\n";
        std::cerr << "\toutWirePtrs_: ";
        for(int j = 0;j<this->pCircuit_->gatePtrs_[i]->outWirePtrs_.size();++j){
            std::cerr << this->pCircuit_->gatePtrs_[i]->outWirePtrs_[j]->name_ << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "Wire info:\n";
    for(int i = 0;i<this->pCircuit_->wirePtrs_.size();++i){
        std::cerr << "Wire " << this->pCircuit_->wirePtrs_[i]->name_ << "\n";
        std::cerr << "\tinGatePtrs_: ";
        for(int j = 0;j<this->pCircuit_->wirePtrs_[i]->inGatePtrs_.size();++j){
            std::cerr << this->pCircuit_->wirePtrs_[i]->inGatePtrs_[j]->name_ << " ";
        }
        std::cerr << "\n";
        std::cerr << "\toutGatePtrs_: ";
        for(int j = 0;j<this->pCircuit_->wirePtrs_[i]->outGatePtrs_.size();++j){
            std::cerr << this->pCircuit_->wirePtrs_[i]->outGatePtrs_[j]->name_ << " ";
        }
        std::cerr << "\n";
    }
}