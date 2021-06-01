#ifndef LINK_H
#define LINK_H
#include "Exp_Node.h"
#include "Rules.h"
#include "Final_NDFA.h"
#include "DFA.h"
#include "Tokens.h"
#include "Minimization.h"
#include "check.h"
using namespace std;

class Link
{
    public:
        Rules rules;
        Final_NDFA ndfa = Final_NDFA();
        DFA dfa;
        Minimization mini;
        Tokens tk;

        check lexical;
        Link();
        string get_next_token();

    protected:

    private:
};

#endif // LINK_H
