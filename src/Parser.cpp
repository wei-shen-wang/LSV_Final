#include "Parser.h"

// void Parser::parseLine(const std::string &line){
//     tokens_.clear();
//     if(!line.empty()){

//     }
//     else{
//         tokenIndex_ = 0;
//     }
// }

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
        this->tokens_.push_back(word);
        if(verilogEndline){
            for(int i = 0;i<tokens_.size();++i){
                std::cerr << tokens_[i] << " ";
            }
            std::cerr << "\n";
            if(tokens_[0] == "module"){

            }
            else if(tokens_[0] == "assign"){

            }
            else if(tokens_[0] == "wire"){

            }
            else if(tokens_[0] == "input"){

            }
            else if(tokens_[0] == "output"){

            }
            else if(tokens_[0] == "not" || tokens_[0] == "buf"){

            }
            else if(tokens_[0] == "nand" || tokens_[0] == "and"
             || tokens_[0] == "nor" || tokens_[0] == "or"
             || tokens_[0] == "xor" || tokens_[0] == "xnor"){

            }
            tokens_.clear();
            verilogEndline = false;
        }
    }
    
}