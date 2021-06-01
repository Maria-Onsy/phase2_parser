#include "Final_NDFA.h"

#include <iostream>
#include "NDFA.h"
#include "Exp_Node.h"

Final_NDFA::Final_NDFA()
{
   num = 0;
}

Final_NDFA::construct_NDFA(list<Exp_Node> expressions){
    list<NDFA> states;
    list<Exp_Node> ::iterator it;
    NDFA temp;
    for(it=expressions.begin();it!=expressions.end();it++){
            Exp_Node ex = *it;
            if(ex.exp){
              temp = temp.get_from_expression(ex.value,this);
              this->get_state(temp.last)->stable = true;
              this->get_state(temp.last)->name = ex.name;
              states.push_back(temp);
            }
            else{
              temp = temp.get_from_non_expression(ex.value,this);
              states.push_back(temp);
            }
    }

   temp=temp.combine(states,this);
   this->start = temp.start;

}


NDFA_state* Final_NDFA::get_state(int id){
  list<NDFA_state> ::iterator it;
  for(it=allStates.begin();it!=allStates.end();it++){
    if((*it).id == id){
        return &(*it);
    }
  }
  return nullptr;
}


NDFA_state* Final_NDFA::add_state(){
  NDFA_state state;
  state.id = num;
  num ++;
  allStates.push_back(state);
  NDFA_state* ptr = &allStates.back();
  return ptr;
}
