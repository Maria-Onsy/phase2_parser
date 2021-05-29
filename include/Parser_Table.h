#ifndef PARSER_TABLE_H
#define PARSER_TABLE_H
#include <string>
#include <list>
using namespace std;
#include "Grammer.h";

class Parser_Table
{
    public:
        Grammer* grammer;
        list<list<int>> table;
        bool nonLL1;
        string error_messege;
        Parser_Table(Grammer* g);

    protected:

    private:
        get_frist(Non_terminal* nonterm);
        get_follow();
        construct_table();
        bool contain(list<int> lt,int id);
};

#endif // PARSER_TABLE_H
