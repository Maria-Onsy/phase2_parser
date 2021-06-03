#ifndef CHECK_H
#define CHECK_H

#include "Tokens.h"
//#include "DFA_state.h"
//#include "DFA.h"
#include <string>
#include <list>
using namespace std;

class check
{
     public:
       class node{
         public:
           string type;
           float value;
           string name;
       };
       list<node> ids;

       //To link with parser
       int num = -1;
       string in = "";
       copy_to(Tokens token);
       string get_next_token(DFA df);



       Check(Tokens token, DFA df);

    private:
        Report_Error(int z);

};

#endif // CHECK_H
