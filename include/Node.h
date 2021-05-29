#ifndef NODE_H
#define NODE_H
#include <string>
#include <list>
using namespace std;

class Node
{
    public:
        string name;
        bool terminal;
        Node(string n , bool t){
            name = n;
            terminal = t;
        }

};

#endif // NODE_H
