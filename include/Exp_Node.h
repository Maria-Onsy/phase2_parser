#ifndef EXP_NODE_H
#define EXP_NODE_H

#include <string>
#include <list>
using namespace std;

class Exp_Node
{
    public:
        string name;
        string value;
        bool exp;
        Exp_Node(string n, string v, bool e){
          name = n;
          value = v;
          exp = e;
        }
};

#endif // EXP_NODE_H
