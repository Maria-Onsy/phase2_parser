#ifndef GRAMMAR_H
#define GRAMMAR_H
#include "Non_terminal.h"
#include <string>
#include <list>
using namespace std;

class Grammar
{
    public:
        string file_name;
        list<Non_terminal> non_terminals;
        list<rule> allRules;
        list<string> terminals;
        int numNonterm;
        int numRule;

        Grammar(string file){
          file_name = file;
          numNonterm = 0;
          numRule = 0;
          terminals.push_back("\\L");
        }

        get_from_file();
        Non_terminal* get_non_terminal(int id);
        Non_terminal* get_non_terminal(string name);
        rule* get_rule(int id);
        int get_terminal_id(string name);
        string get_terminal(int id);

    private:
        bool termContain(string name);
};

#endif // GRAMMER_H
