#ifndef GATE_H
#define GATE_H
#include <string>
class Wire;
class Gate;
class Gate{
public:
    enum Type{
        PI=0, 
        PO,
        NOT,
        BUF,
        NAND,
        AND,
        NOR,
        OR,
        XOR,
        XNOR
    };
    inline Gate(){

    }
    inline ~Gate(){

    }
    int index_;
    std::string name_;
    Type type_;
    std::vector<Wire*> inWirePtrs_;
    std::vector<Wire*> outWirePtrs_;

    bool inserted_;
    bool camouflaged_;

};

class Wire
{
public:
    inline Wire(/* args */){

    }
    inline ~Wire(){

    }
    int index_;
    int level_;
    std::string name_;
    std::vector<Gate*> inGatePtrs_;
    std::vector<Gate*> outGatePtrs_;

};

#endif