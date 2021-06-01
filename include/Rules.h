#ifndef RULES_H
#define RULES_H

#include "Exp_Node.h"
#include <string>
#include <list>
using namespace std;

class Rules
{
    public:
        list<Exp_Node> expressions;
        list<string> rule;
        string path;
        list<string> priority;
        read_from_file();
        convert_to_expressions();
        string get_priority(list<string> names);
    private:
        list<Exp_Node> definitions;
        resolve_definitions();
};

#endif // RULES_H
