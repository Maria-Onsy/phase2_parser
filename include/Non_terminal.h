#ifndef NON_TERMINAL_H
#define NON_TERMINAL_H
#include "Node.h"
#include"rule.h"
#include <string>
#include <list>
using namespace std;


class Non_terminal
{
    public:
        string name;
        int id;
        list<rule> to;
        list<string> first;
        list<string> follow;

        Non_terminal(int num, string n, list<rule> rules){
           id = num;
           name = n;
           to = rules;
        }
};

#endif // NON_TERMINAL_H
