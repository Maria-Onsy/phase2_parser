#include <iostream>
#include "Grammer.h"
#include "Parser_Table.h"
#include "Parser.h"

using namespace std;

int main()
{
    Grammer g = Grammer("grammer.txt");
    g.get_from_file();

/*
    list<Non_terminal>::iterator it;
    for(it=g.non_terminals.begin();it!=g.non_terminals.end();it++){
        cout<<"Rule:\n";
        list<rule>::iterator it2;
        for(it2=(*it).to.begin();it2!=(*it).to.end();it2++){
            Non_terminal* tr= g.get_non_terminal((*it2).non_terminal);
            cout<<tr->name;
            cout<<" = ";
            list<Node>::iterator it3;
            for(it3=(*it2).to.begin();it3!=(*it2).to.end();it3++){
                if((*it3).terminal){
                    cout<< "\'";
                    cout << (*it3).name;
                    cout<< "\'";
                }
                else{cout << (*it3).name;}
                cout<< " ";
            }
            cout<<"\n";
        }
    }
*/

/*int k=0;
list<string>::iterator ir;
for(ir=g.terminals.begin();ir!=g.terminals.end();ir++){
    cout<<k;
    cout<<"  ";
    cout<<(*ir);
    cout<<"\n";
    k++;
}*/


  Parser_Table table = Parser_Table(&g);
  if(table.nonLL1){
    cerr<<table.error_messege;
    cout<<"\n";
    return 0;
  }

/*list<Non_terminal>::iterator it;
for(it=g.non_terminals.begin();it!=g.non_terminals.end();it++){
    cout<<"Non terminal:\n";
    cout<<(*it).name;
    cout<<"\n";
    cout<< "first:";
    cout<<"\n";
    list<int>::iterator f;
    for(f=(*it).first.begin();f!=(*it).first.end();f++){
        cout<<(*f);
        cout<<"     ";
    }
    cout<<"\n";
}*/


Parser p = Parser(table.table);
p.path = "input.txt";
    return 0;
}
