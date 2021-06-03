#include "Parser_Table.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>

Parser_Table::Parser_Table(Grammar* g){
  grammar = g;

  //compute first
  list<Non_terminal>::iterator it;
  for(it=this->grammar->non_terminals.begin();it!=this->grammar->non_terminals.end();it++){
    if((*it).first.empty()){
    get_frist(&(*it));
    }
  }

  //compute follow
  for(it=this->grammar->non_terminals.begin();it!=this->grammar->non_terminals.end();it++){
    if((*it).follow.empty()){
    get_follow(&(*it));
    }
  }

  construct_table();

  write_table();

}

Parser_Table::get_frist(Non_terminal* nonterm){

        list<rule>::iterator itr;
        for(itr=nonterm->to.begin();itr!=nonterm->to.end();itr++){
            Node n = (*itr).to.front();

            //if it is a terminal
            if(n.terminal){
                nonterm->first.push_back(pair<int,int>(grammar->get_terminal_id(n.name),(*itr).id));
            }
            //if it is a nonterminal
            else{
                Non_terminal* nont2 = grammar->get_non_terminal(n.name);
                if(nont2->first.empty()){
                   get_frist(nont2);
                }
                list<pair<int,int>>::iterator it;
                for(it=nont2->first.begin();it!=nont2->first.end();it++){
                    if(!contain(nonterm->first,(*it).first)){
                       nonterm->first.push_back(pair<int,int>((*it).first,(*itr).id));
                    }
                }
            }
        }

}

Parser_Table::get_follow(Non_terminal* nonterminal){

    //The start nonterminal
    if (nonterminal->id == 0){
        nonterminal->follow.push_back(-1);  //add $ to the follow
    }

    list<rule>::iterator it;
    for(it = grammar->allRules.begin(); it != grammar->allRules.end(); it++){
        list<Node>::iterator n;
        for(n = it->to.begin(); n != it->to.end(); n++){
            if(n->name.compare(nonterminal->name) == 0 && std::next(n,1) != it->to.end() ){
                //if the next node is terminal
                if(std::next(n,1)->terminal){
                    int id = grammar->get_terminal_id(std::next(n,1)->name);
                   if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), id) != nonterminal->follow.end()) && id != 0){
                         nonterminal->follow.push_back(id);
                    }
                }
               //if the next node is nonterminal
                else{
                    Non_terminal* nt = grammar->get_non_terminal(std::next(n,1)->name);
                    list<pair<int,int>>::iterator y;
                    for(y = nt->first.begin(); y != nt->first.end(); y++){
                        if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y).first) != nonterminal->follow.end()) && (*y).first!=0 ){
                            nonterminal->follow.push_back((*y).first);
                        }
                    }
                    //if the next node contains epsilon in its first
                    if(contain(nt->first,0/*id of eps*/)){
                        list<int>::iterator y1;
                        Non_terminal* nonterm = grammar->get_non_terminal(it->non_terminal);
                        for(y1 = nonterm->follow.begin(); y1 != nonterm->follow.end(); y1++){
                            if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y1)) != nonterminal->follow.end())&& (*y1)!=0 ){
                                nonterminal->follow.push_back(*y1);
                            }
                        }
                    }
                }
            }
            //if it is the right most node in the rule
            else if(n->name.compare(nonterminal->name) == 0 && std::next(n,1) == it->to.end()){
                Non_terminal* ntt = grammar->get_non_terminal(it->non_terminal);
                list<int>::iterator y;
                for(y = ntt->follow.begin(); y != ntt->follow.end(); y++){
                     if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y)) != nonterminal->follow.end())&& (*y)!=0 ){
                        nonterminal->follow.push_back(*y);
                    }
                }
            }
        }
    }
}

Parser_Table::construct_table(){

    list<Non_terminal>:: iterator i;
    for(i=grammar->non_terminals.begin(); i!=grammar->non_terminals.end(); i++){
        list<pair<int,int>>:: iterator j;
        bool contepsilon = false;
        int epsilonrule;
        list<int> l;
        list<int>:: iterator k;
        int q=0; int r;
        for(k=l.begin(); q<grammar->terminals.size()+1; k++){q++; l.push_back(-3);}
        for(j=i->first.begin(); j!=i->first.end(); j++){
            if((*j).first == 0){contepsilon=true; epsilonrule = (*j).second;}
            else{
                k=l.begin();
                advance(k, (*j).first);
                if((*k)==-3){k=l.erase(k); l.insert(k, (*j).second);}
                else{
                    notLL1 = true;
                    error_message = "ERROR: This grammer is not a LL(1) grammar\nnomterminal " + (*i).name
                                    + "has more than one production under the input " + grammar->get_terminal((*j).first);
                        return 0;}
            }
        }
        if(contepsilon){
            list<int>:: iterator m;
            for(m=i->follow.begin(); m!=i->follow.end(); m++){
                k=l.begin();
                if((*m)==-1){(*m)=grammar->terminals.size();}
                advance(k, (*m));
                if((*k)==-3){k=l.erase(k); l.insert(k, epsilonrule);}
                else{
                    notLL1 = true;
                    string tr;
                    if((*m)==-1){tr = "$";}
                    else{tr = grammar->get_terminal(*m);}
                    error_message = "ERROR: This grammer is not a LL(1) grammar\nnomterminal " + (*i).name
                                    + "has more than one production under the input " + tr;
                    return 0;}
            }
        }
        else{
            list<int>:: iterator m;
            for(m=i->follow.begin(); m!=i->follow.end(); m++){
                k=l.begin();
                if((*m)==-1){(*m)=grammar->terminals.size();}
                advance(k, (*m));
                if((*k)==-3){k=l.erase(k); l.insert(k, -2);}
            }
        }

        list<list<int>>:: iterator tit = table.begin();
        advance(tit,(*i).id);
        table.push_back(l);
    }

}

Parser_Table::write_table(){

    fstream file;
    string text;
    file.open("Parser Table.txt",ios::out);
    if(!file){
        cerr << "Error: file could not be created" << endl;
        exit(1);
    }

    if(notLL1){
        file << error_message<<endl;
        file.close();
        return 0;
    }

    list<list<int>>::iterator itt;
    list<int>::iterator itt2;
    int ito = 0;
    for(itt=table.begin();itt!=table.end();itt++){
        file<< "Nonterminal:     ";
        file<<grammar->get_non_terminal(ito)->name<<endl;
        int jtn = 0;
        for(itt2=(*itt).begin();itt2!=(*itt).end();itt2++){
            if(jtn==grammar->terminals.size()){file<<"$";}
            else{file<<grammar->get_terminal(jtn);}
            file<<" -->  ";
            if((*itt2)== -3){ file<<"error";}
            else if((*itt2)== -2){ file<<"sync";}
            else{
                string temp="";
                rule* r = grammar->get_rule(*itt2);
                file<<grammar->get_non_terminal(ito)->name;
                file<< " = ";
                list<Node>::iterator itt3;
                for(itt3=r->to.begin();itt3!=r->to.end();itt3++){
                    if((*itt3).terminal){
                        temp+= "\'" + (*itt3).name + "\'";
                }
                else{temp+= (*itt3).name;}
                temp+= " ";
            }
           file<<temp;
            }
        file<<"\n";
        jtn++;
        }
        file<<"--------------------------------------------------------------\n";
        ito++;
    }

    file.close();
}

bool Parser_Table::contain(list<pair<int,int>> lt,int id){
  list<pair<int,int>>::iterator it;
  for(it=lt.begin();it!=lt.end();it++){
        if((*it).first == id){
            return true;
        }
  }
  return false;
}
