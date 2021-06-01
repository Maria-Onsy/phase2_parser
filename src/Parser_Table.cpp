#include "Parser_Table.h"
#include <iostream>
#include <algorithm>
#include <string.h>

Parser_Table::Parser_Table(Grammer* g){
  grammer = g;

  list<Non_terminal>::iterator it;
  for(it=this->grammer->non_terminals.begin();it!=this->grammer->non_terminals.end();it++){
    if((*it).first.empty()){
    get_frist(&(*it));
    }
  }
  for(it=this->grammer->non_terminals.begin();it!=this->grammer->non_terminals.end();it++){
    if((*it).follow.empty()){
    get_follow(&(*it));
    }
  }

  construct_table();
}

Parser_Table::get_frist(Non_terminal* nonterm){

        list<rule>::iterator itr;
        for(itr=nonterm->to.begin();itr!=nonterm->to.end();itr++){
            Node n = (*itr).to.front();
            if(n.terminal){
                //int temp [2] = {grammer->get_terminal_id(n.name),(*itr).id};
                nonterm->first.push_back(pair<int,int>(grammer->get_terminal_id(n.name),(*itr).id));
            }
            else{
                Non_terminal* nont2 = grammer->get_non_terminal(n.name);
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


    if (nonterminal->id == 0){
        nonterminal->follow.push_back(-1);
    }

    list<rule>::iterator it;
    for(it = grammer->allRules.begin(); it != grammer->allRules.end(); it++){
        list<Node>::iterator n;
        for(n = it->to.begin(); n != it->to.end(); n++){
            //Node next = *(std::next(n,1));
            if(n->name.compare(nonterminal->name) == 0 && std::next(n,1) != it->to.end() ){
                if(std::next(n,1)->terminal){
                    int id = grammer->get_terminal_id(std::next(n,1)->name);
                   if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), id) != nonterminal->follow.end()) && id != 0){
                         nonterminal->follow.push_back(id);
                    }
                }
                else{
                    Non_terminal* nt = grammer->get_non_terminal(std::next(n,1)->name);
                    list<pair<int,int>>::iterator y;
                    for(y = nt->first.begin(); y != nt->first.end(); y++){
                        if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y).first) != nonterminal->follow.end()) && (*y).first!=0 ){
                            nonterminal->follow.push_back((*y).first);
                        }
                    }
                    if(contain(nt->first,0/*id of eps*/)){
                        list<int>::iterator y1;
                        //int nontermID = grammer->it->non_terminal;
                        Non_terminal* nonterm = grammer->get_non_terminal(it->non_terminal);
                        for(y1 = nonterm->follow.begin(); y1 != nonterm->follow.end(); y1++){
                            if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y1)) != nonterminal->follow.end())&& (*y1)!=0 ){
                                nonterminal->follow.push_back(*y1);
                            }
                        }
                    }
                }
            }
            else if(n->name.compare(nonterminal->name) == 0 && std::next(n,1) == it->to.end()){
                Non_terminal* ntt = grammer->get_non_terminal(it->non_terminal);
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
    for(i=grammer->non_terminals.begin(); i!=grammer->non_terminals.end(); i++){
        list<pair<int,int>>:: iterator j;
        bool contepsilon = false;
        int epsilonrule;
        list<int> l;
        list<int>:: iterator k;
        int q=0; int r;
        for(k=l.begin(); q<grammer->terminals.size()+1; k++){q++; l.push_back(-3);}
        for(j=i->first.begin(); j!=i->first.end(); j++){
            if((*j).first == 0){contepsilon=true; epsilonrule = (*j).second;}
            else{
                k=l.begin();
                advance(k, (*j).first);
                if((*k)==-3){k=l.erase(k); l.insert(k, (*j).second);}
                else{cout << "ERROR: This grammer is ambigous" <<endl; return 0;}
            }
        }
        if(contepsilon){
            list<int>:: iterator m;
            for(m=i->follow.begin(); m!=i->follow.end(); m++){
                k=l.begin();
                if((*m)==-1){(*m)=grammer->terminals.size();}
                advance(k, (*m));
                if((*k)==-3){k=l.erase(k); l.insert(k, epsilonrule);}
                else{cout << "ERROR: This grammer is ambigous" <<endl; return 0;}
            }
        }
        else{
            list<int>:: iterator m;
            for(m=i->follow.begin(); m!=i->follow.end(); m++){
                k=l.begin();
                if((*m)==-1){(*m)=grammer->terminals.size();}
                advance(k, (*m));
                if((*k)==-3){k=l.erase(k); l.insert(k, -2);}
            }
        }

        list<list<int>>:: iterator tit = table.begin();
        advance(tit,(*i).id);
        table.push_back(l);
    }

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
