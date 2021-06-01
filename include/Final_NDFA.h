#ifndef FINAL_NDFA_H
#define FINAL_NDFA_H

#include "Exp_Node.h"
#include "NDFA_state.h"
#include <list>
using namespace std;

class Final_NDFA
{
    public:
        int num;
        list<NDFA_state> allStates;
        int start;
        Final_NDFA();
        construct_NDFA(list<Exp_Node> expressions);
        NDFA_state* get_state(int id);
        NDFA_state* add_state();
};

#endif // FINAL_NDFA_H
