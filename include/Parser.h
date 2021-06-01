#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <list>
#include <stack>
#include "Grammer.h"
using namespace std;


class Parser
{
    public:
        Grammer* g;
        list<list<int>> table;
        list<string> output;
        stack<pair<int,bool>> st;
      //  stack<int> outIndx;
        int input;
        bool getNext = true;

        //without linking to lexical
        list<string> file;

        Parser(list<list<int>> t,Grammer* gr ){
          table = t;
          g = gr;
          st.push(pair<int,bool>(-1,true));
          st.push(pair<int,bool>(0,false));
        //  outIndx.push(0);

        };

        parse();
        error_recovery();

        //without linking to lexical
        add_input();


    private:
        bool get_next_token();

};

#endif // PARSER_H
