#ifndef DFA_H
#define DFA_H

//#include "Exp_Node.h"
#include "Final_NDFA.h"
#include "DFA_state.h"
#include "NDFA_state.h"
#include"Rules.h"
//#include "NDFA.h"

#include <list>
using namespace std;

class DFA
{
public:
    int num=0;
    DFA_state* tmpstate;
    list<DFA_state> allStates;
    int start;

    construct_DFA(Final_NDFA nd,Rules rl);
    list<NDFA_state> getEps(Final_NDFA nd ,int state);
    list<DFA_state> getstates();
    bool exists (list<int>);
    bool equalst (list<int> next,DFA_state* st);
    DFA_state* get_state(int id);
    DFA_state* add_state();
};

#endif // DFA_H
