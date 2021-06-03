#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <list>
#include <stack>
#include "Grammar.h"
#include "Link.h"
using namespace std;


class Parser
{
    public:
        Grammar* g;
        Link linker = Link();
        list<list<int>> table;
        list<string> output;
        stack<pair<int,bool>> st;
        int input;
        bool getNext = true;

        Parser(list<list<int>> t,Grammar* gr ){
          table = t;
          g = gr;
          st.push(pair<int,bool>(-1,true));
          st.push(pair<int,bool>(0,false));
        }

        parse();
        writeFile();



    private:
        bool get_next_token();

};

#endif // PARSER_H
