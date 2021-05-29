#include "Parser_Table.h"
#include <iostream>

Parser_Table::Parser_Table(Grammer* g){
  grammer = g;
  nonLL1 = false;

  list<Non_terminal>::iterator it;
  for(it=this->grammer->non_terminals.begin();it!=this->grammer->non_terminals.end();it++){
    if((*it).first.empty()){
    get_frist(&(*it));
    }
  }

  get_follow();

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

Parser_Table::get_follow(){

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
