#ifndef NDFA_STATE_H
#define NDFA_STATE_H

#include <list>
#include <string>
using namespace std;


class NDFA_state
{
    public:
       class line{
         public:
           int to;
           string input;
           bool str = false;
       };
       int id;
       bool stable;
       string name;
       list<line> trans;

     bool operator == (const NDFA_state & nd);
};



#endif // NDFA_STATE_H
