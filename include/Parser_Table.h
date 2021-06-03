#ifndef PARSER_TABLE_H
#define PARSER_TABLE_H
#include <string>
#include <list>
using namespace std;
#include "Grammar.h";

class Parser_Table
{
    public:
        Grammar* grammar;
        bool notLL1 = false;
        string error_message;
        list<list<int>> table;

        Parser_Table(Grammar* g);


    private:
        get_frist(Non_terminal* nonterm);
        get_follow(Non_terminal* nonterm);
        construct_table();
        write_table();
        bool contain(list<pair<int,int>> lt,int id);
};

#endif // PARSER_TABLE_H
