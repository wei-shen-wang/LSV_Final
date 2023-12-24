#ifndef GATE_H
#define GATE_H
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
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
    std::unordered_map<Type, std::string> type2String = {
        {PI, "PI"},
        {PO, "PO"},
        {NOT, "NOT"},
        {BUF, "BUF"},
        {NAND, "NAND"},
        {AND, "AND"},
        {NOR, "NOR"},
        {OR, "OR"},
        {XOR, "XOR"},
        {XNOR, "XNOR"}
    };
    inline Gate(const std::string &name, Type type){
        name_ = name;
        type_ = type;
        inserted_ = false;
        camouflaged_ = false;
        topoSorted_ = false;

    }
    inline ~Gate(){

    }
    inline const std::string getTypeString(){
        return type2String[type_];
    }
    bool topoSorted_;
    int index_;
    int level_;
    std::string name_;
    Type type_;
    std::vector<Wire*> inWirePtrs_;
    std::vector<Wire*> outWirePtrs_;
    int keyBitVal_;

    bool outputInserted_;
    bool inserted_;
    bool camouflaged_;

};

class Wire
{
public:
    inline Wire(const std::string &name){
        name_ = name;
        level_ = -1;
        topoSorted_ = false;
        outputInserted_ = false;
        inserted_ = false;
    }
    inline ~Wire(){

    }
    bool inserted_;
    bool outputInserted_;
    int potentialKeyHardness_;
    std::unordered_set<Gate*> POGatePtrs_;

    bool topoSorted_;
    int index_;
    int level_;
    std::string name_;
    Gate* inGatePtr_;
    std::vector<Gate*> outGatePtrs_;

};

#endif