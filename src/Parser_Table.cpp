#include "Parser_Table.h"
#include <iostream>
#include <algorithm>
#include <string.h>

Parser_Table::Parser_Table(Grammer* g){
  grammer = g;
  nonLL1 = false;

  list<Non_terminal>::iterator it;
  for(it=this->grammer->non_terminals.begin();it!=this->grammer->non_terminals.end();it++){
    if((*it).first.empty()){
    get_frist(&(*it));
    }
  }
  for(it=this->grammer->non_terminals.begin();it!=this->grammer->non_terminals.end();it++){
        cout << "Non Terminal: " << it->name << endl;
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
                nonterm->first.push_back(grammer->get_terminal_id(n.name));
            }
            else{
                Non_terminal* nont2 = grammer->get_non_terminal(n.name);
                if(nont2->first.empty()){
                   get_frist(nont2);
                }
                list<int>::iterator it;
                for(it=nont2->first.begin();it!=nont2->first.end();it++){
                    if(!contain(nonterm->first,(*it))){
                       nonterm->first.push_back((*it));
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
                    list<int>::iterator y;
                    for(y = nt->first.begin(); y != nt->first.end(); y++){
                        if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y)) != nonterminal->follow.end()) && (*y)!=0 ){
                            nonterminal->follow.push_back(*y);
                        }
                    }
                    if(std::find(nt->first.begin(), nt->first.end(), 0/*id of eps*/) != nt->first.end()){
                        //int nontermID = grammer->it->non_terminal;
                        Non_terminal* nonterm = grammer->get_non_terminal(it->non_terminal);
                        for(y = nonterm->follow.begin(); y != nonterm->follow.end(); y++){
                            if(!(std::find(nonterminal->follow.begin(), nonterminal->follow.end(), (*y)) != nonterminal->follow.end())&& (*y)!=0 ){
                                nonterminal->follow.push_back(*y);
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

}

bool Parser_Table::contain(list<int> lt,int id){
  list<int>::iterator it;
  for(it=lt.begin();it!=lt.end();it++){
        if((*it) == id){
            return true;
        }
  }
  return false;
}
