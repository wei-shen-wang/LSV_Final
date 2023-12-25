#include "Parser.h"

void Parser::writeVerilog(){
    this->filename_.erase(this->filename_.begin(),this->filename_.begin() + this->filename_.find_last_of("/") + 1);
    this->filename_.resize(this->filename_.size() - 2);
    std::string path = this->filename_ + "_out.v";
    std::ofstream outFile(path, std::ofstream::out);
    if (!outFile) {
        std::cerr << "cannot open output file";
        std::cerr << path << "\n";
        exit(1);
    }
    outFile << "module " << this->pCircuit_->name_ << "(";
    for (int i = 0; i < int(this->pCircuit_->inputNames_.size()); ++i) {
        outFile << *std::next(this->pCircuit_->inputNames_.begin(), i);
        outFile << ", ";
    }
    for (int i = 0; i < int(this->pCircuit_->insertedKeyWirePtrs_.size()); ++i) {
        outFile << "key_" << i;
        outFile << ", ";
    }
    for (int i = 0; i < int(this->pCircuit_->outputNames_.size()); ++i) {
        outFile << *std::next(this->pCircuit_->outputNames_.begin(), i);
        if (i != int(this->pCircuit_->outputNames_.size()) - 1)
        {
            outFile << ", ";
        }
    }
    outFile << ");\n";
    outFile << "\tinput ";
    for (int i = 0; i < int(this->pCircuit_->inputNames_.size()); ++i) {
        outFile << *std::next(this->pCircuit_->inputNames_.begin(), i);
        if(i != int(this->pCircuit_->inputNames_.size()) - 1){
            outFile << ", ";
        }
    }
    outFile << ";\n";
    if (this->pCircuit_->insertedKeyWirePtrs_.size() != 0)
    {
        outFile << "\tinput ";
        for (int i = 0; i < int(this->pCircuit_->insertedKeyWirePtrs_.size()); ++i) {
            outFile << "key_" << i;
            if(i != int(this->pCircuit_->insertedKeyWirePtrs_.size()) - 1){
                outFile << ", ";
            }
        }
        outFile << ";\n";
    }
    outFile << "\toutput ";
    for (int i = 0; i < int(this->pCircuit_->outputNames_.size()); ++i)
    {
        outFile << *std::next(this->pCircuit_->outputNames_.begin(), i);
        if(i != int(this->pCircuit_->outputNames_.size()) - 1){
            outFile << ", ";
        }
    }
    outFile << ";\n";
    outFile << "\twire ";
    for (int i = 0; i < int(this->pCircuit_->wirePtrs_.size()); ++i)
    {
        outFile << this->pCircuit_->wirePtrs_[i]->name_;
        if(i != int(this->pCircuit_->wirePtrs_.size()) - 1){
            outFile << ", ";
        }
    }
    outFile << ";\n";
    if (this->pCircuit_->insertedKeyWirePtrs_.size() != 0)
    {
        outFile << "\twire ";
        for (int i = 0; i < int(this->pCircuit_->insertedKeyWirePtrs_.size()); ++i)
        {
            outFile << this->pCircuit_->insertedKeyWirePtrs_[i]->name_;
            if(i != int(this->pCircuit_->insertedKeyWirePtrs_.size()) - 1){
                outFile << ", ";
            }
        }
        outFile << ";\n";
        outFile << "\twire ";
        for (int i = 0; i < int(this->pCircuit_->insertedKeyWirePtrs_.size()); ++i)
        {
            outFile << "key_" << i;
            if(i != int(this->pCircuit_->insertedKeyWirePtrs_.size()) - 1){
                outFile << ", ";
            }
        }
        outFile << ";\n";
    }

    for (int i = 0; i < int(this->pCircuit_->gatePtrs_.size()); ++i)
    {
        Gate* pGate = this->pCircuit_->gatePtrs_[i];
        if(pGate->type_ == Gate::PI || pGate->type_ == Gate::PO){
            continue;
        }
        else{
            outFile << "\tassign " << pGate->outWirePtrs_[0]->name_ << " = ";
            switch (pGate->type_)
            {
            case Gate::NOT:
                outFile << "~" << pGate->inWirePtrs_[0]->name_;
                break;
            case Gate::BUF:
                outFile << pGate->inWirePtrs_[0]->name_;
                break;
            case Gate::NAND:
                outFile << "~( " << pGate->inWirePtrs_[0]->name_;
                for (int i = 1; i < int(pGate->inWirePtrs_.size()); ++i)
                {
                    outFile << " & " << pGate->inWirePtrs_[i]->name_;
                }
                outFile << " )";
                break;
            case Gate::AND:
                outFile << "( " << pGate->inWirePtrs_[0]->name_;
                for (int i = 1; i < int(pGate->inWirePtrs_.size()); ++i)
                {
                    outFile << " & " << pGate->inWirePtrs_[i]->name_;
                }
                outFile << " )";
                break;
            case Gate::NOR:
                outFile << "~( " << pGate->inWirePtrs_[0]->name_;
                for (int i = 1; i < int(pGate->inWirePtrs_.size()); ++i)
                {
                    outFile << " | " << pGate->inWirePtrs_[i]->name_;
                }
                outFile << " )";
                break;
            case Gate::OR:
                outFile << "( " << pGate->inWirePtrs_[0]->name_;
                for (int i = 1; i < int(pGate->inWirePtrs_.size()); ++i)
                {
                    outFile << " | " << pGate->inWirePtrs_[i]->name_;
                }
                outFile << " )";
                break;
            case Gate::XOR:
                outFile << "( " <<  pGate->inWirePtrs_[0]->name_;
                for (int i = 1; i < int(pGate->inWirePtrs_.size()); ++i)
                {
                    outFile << " ^ " << pGate->inWirePtrs_[i]->name_;
                }
                outFile << " )";
                break;
            case Gate::XNOR:
                outFile << "~( " << pGate->inWirePtrs_[0]->name_;
                for (int i = 1; i < int(pGate->inWirePtrs_.size()); ++i)
                {
                    outFile << " ^ " << pGate->inWirePtrs_[i]->name_;
                }
                outFile << " )";
                break;
            default:
                break;
            }
            outFile << ";\n";
        }
    }
    for (int i = 0; i < int(this->pCircuit_->insertedKeyGatePtrs_.size()); ++i)
    {
        Gate *pGate = this->pCircuit_->insertedKeyGatePtrs_[i];
        if (pGate->type_ == Gate::PI || pGate->type_ == Gate::PO)
        {
            continue;
        }
        else
        {
            outFile << "\tassign " << pGate->outWirePtrs_[0]->name_ << " = ";
            switch (pGate->type_)
            {
                case Gate::XOR:
                    outFile << "( " << pGate->inWirePtrs_[0]->name_;
                    outFile << " ^ " << "key_" << i;
                    outFile << " )";
                    break;
                case Gate::XNOR:
                    outFile << "~( " << pGate->inWirePtrs_[0]->name_;
                    outFile << " ^ " << "key_" << i;
                    outFile << " )";
                    break;
                default:
                    break;
            }
            outFile << ";\n";
        }
    }
    outFile << "endmodule\n";
    outFile.close();
}