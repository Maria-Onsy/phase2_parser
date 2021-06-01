#ifndef NDFA_H
#define NDFA_H

#include "Final_NDFA.h"
#include "NDFA_state.h"
#include "Exp_Node.h"
#include <string>
#include <list>
using namespace std;


class NDFA
{
    public:
       int start;
       int last;
       NDFA get_from_expression(string exp,Final_NDFA* fNdfa);
       NDFA get_from_non_expression(string exp,Final_NDFA* fNdfa);
       NDFA combine(list<NDFA> states,Final_NDFA* fNdfa);

};

#endif // NDFA_H
