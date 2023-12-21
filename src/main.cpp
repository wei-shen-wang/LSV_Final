#include <iostream>
#include "Parser.h"

int main(int argc, char *argv[]){
    std::cout << "Hi mama\n";
    std::string filename = argv[1];
    Parser parser(filename);
    parser.parseFile();
}