#ifndef RULE_H
#define RULE_H
#include "Node.h"
#include <string>
#include <list>
using namespace std;

class rule
{
    public:
        int id;
        int non_terminal;
        list<Node> to;

        rule(int num , int from, list<Node> nodes){
          id = num;
          non_terminal = from;
          to = nodes;
        }

};

#endif // RULE_H
