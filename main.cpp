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

/*
int k=0;
list<string>::iterator ir;
for(ir=g.terminals.begin();ir!=g.terminals.end();ir++){
    cout<<k;
    cout<<"  ";
    cout<<(*ir);
    cout<<"\n";
    k++;
}
*/


  Parser_Table table = Parser_Table(&g);


  /*
list<Non_terminal>::iterator it;
for(it=g.non_terminals.begin();it!=g.non_terminals.end();it++){
    cout<<"Non terminal:\n";
    cout<<(*it).name;
    cout<<"\n";
    cout<< "first:";
    cout<<"\n";
    list<pair<int,int>>::iterator f;
    for(f=(*it).first.begin();f!=(*it).first.end();f++){
        cout<<(*f).first;
        cout<<" -->rule ";
        cout<<(*f).second;
        cout<<"     ";
    }
    cout<<"\n";
}
*/

//cout<<"\n\n\n\n";

/*
list<Non_terminal>::iterator it2;
for(it2=g.non_terminals.begin();it2!=g.non_terminals.end();it2++){
    cout<<"Non terminal:\n";
    cout<<(*it2).name;
    cout<<"\n";
    cout<< "follow:";
    cout<<"\n";
    list<int>::iterator f;
    for(f=(*it2).follow.begin();f!=(*it2).follow.end();f++){
        cout<<(*f);
        cout<<"     ";
    }
    cout<<"\n";
}
*/

/*
list<list<int>>::iterator itt;
list<int>::iterator itt2;
int ito = 0;
for(itt=table.table.begin();itt!=table.table.end();itt++){
    cout<<"Nonterminal:     ";
    cout<<g.get_non_terminal(ito)->name;
    cout<<"\n";
    int jtn = 0;
    for(itt2=(*itt).begin();itt2!=(*itt).end();itt2++){
    if(jtn==g.terminals.size()){cout<<"$";}
    else{cout<<g.get_terminal(jtn);}
    cout<<" --> ";
    if((*itt2)== -3){cout<<"error";}
    else if((*itt2)== -2){cout<<"sync";}
    else{
       string temp="";
       rule* r = g.get_rule(*itt2);
       list<Node>::iterator itt3;
            for(itt3=r->to.begin();itt3!=r->to.end();itt3++){
                if((*itt3).terminal){
                    temp+= "\'";
                    temp+= (*itt3).name;
                    temp+= "\'";
                }
                else{temp+= (*itt3).name;}
                temp+= " ";
            }
       cout<<temp;
    }
    cout<<"\n";
    jtn++;
}
cout<<"-------------------------------------------------\n";
ito++;
}
*/

Parser p = Parser(table.table,&g);

//to test without linking to lexical
p.add_input();

p.parse();
/*
list<string>::iterator st;
for(st=p.output.begin();st!=p.output.end();st++){
    cout<<(*st);
    cout<<"\n";
}
*/

p.writeFile();
p.linker.tk.write_to_file();


    return 0;
}
