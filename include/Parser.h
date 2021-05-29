#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <list>
#include <stack>
using namespace std;


class Parser
{
    public:
        string path;
        list<list<int>> table;
        list<string> output;
        stack<string> st;
        stack<string> input;

        Parser(list<list<int>> t){
          table = t;
          st.push("$");
          st.push("0");
          input.push("$");
        };

        parse();


    private:
        get_next_token();
};

#endif // PARSER_H
