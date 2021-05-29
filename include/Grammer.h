#ifndef GRAMMER_H
#define GRAMMER_H
#include "Non_terminal.h"
#include <string>
#include <list>
using namespace std;

class Grammer
{
    public:
        string file_name;
        list<Non_terminal> non_terminals;
        list<rule> allRules;
        list<string> terminals;
        int numNonterm;
        int numRule;

        Grammer(string file){
          file_name = file;
          numNonterm = 0;
          numRule = 0;
        }

        get_from_file();
        Non_terminal* get_non_terminal(int id);
        rule* get_rule(int id);

    private:
        bool termContain(string name);
};

#endif // GRAMMER_H
