#include "Circuit.h"

void Circuit::insertInitialRandomKeyGates(){
    // random generate 10% key gate
    // all xnor gates
    // calculate key gate insertion score
}

int Circuit::findKeyGateMetric(Gate* gate1Ptr, Gate* gate2Ptr){
    int metric = 0;
    // if isolated, metric = 0
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

void Circuit::insertKeyGates(){
    // insert initial random key gates
    // while key gate number < 5% of total gate number
        // for each wire output not inserted with key gate
            // for each key gate
                // wire.potential key hardness += calculate key gate metric
        // choose wire with highest potential key hardness and insert xor (key gate)
    // change key gate value
    // merge key gate to original circuit
}