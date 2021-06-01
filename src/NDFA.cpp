#include "NDFA.h"

#include <iostream>

#include "NDFA_state.h"
#include <string>
#include <list>
using namespace std;

NDFA NDFA::get_from_expression(string exp,Final_NDFA* fNdfa){
    list<NDFA> temp;
    list<NDFA> ors;
    bool orr = false;
   for(int i=0;i<exp.length();i++){

    if(exp[i]=='('){
           int k;
           int braces=1;
          for(int j=i+1;j<exp.length();j++){
            if(exp[j]=='('){
                braces++;
               }
            if(exp[j]==')'){
                braces--;
                if(braces==0){
                k=j;
                break;}
                }
          }
          string t = exp.substr(i+1,k-i-1);
          temp.push_back(get_from_expression(t,fNdfa));
          i=k;
       }

    else if(exp[i]=='+'){
        NDFA prev = temp.back();
        NDFA_state* en = fNdfa->get_state(prev.last);
        NDFA_state::line ltemp;
        ltemp.input =" ";
        ltemp.to = prev.start;
        en->trans.push_back(ltemp);
       }

    else if(exp[i]=='*'){
        NDFA prev = temp.back();
        NDFA_state* en = fNdfa->get_state(prev.last);
        NDFA_state::line ltemp1;
        ltemp1.input =" ";
        ltemp1.to = prev.start;
        en->trans.push_back(ltemp1);

        NDFA_state* newSt = fNdfa->add_state();
        newSt->stable = false;
        newSt->trans.push_back(ltemp1);
        temp.back().start = newSt->id;

        NDFA_state* newen = fNdfa->add_state();
        newen->stable = false;
        NDFA_state::line ltemp2;
        ltemp2.input =" ";
        ltemp2.to = newen->id;
        en->trans.push_back(ltemp2);
        temp.back().last = newen->id;

        NDFA_state::line ltemp3;
        ltemp3.input = " ";
        ltemp3.to = newen->id;
        newSt->trans.push_back(ltemp3);
       }

    else if(exp[i]=='|'){
        orr = true;
        ors.push_back(temp.back());
        temp.pop_back();
       }

    else{
        bool str =false;
        string lt;
        if(exp[i]=='\\'){
           if(exp[i+1]=='L'){
                exp[i+1] = ' ';
           }
           else if((exp[i+1]=='A')){
            str = true;
            lt = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
           }
            else if((exp[i+1]=='a')){
            str = true;
            lt = "abcdefghijklmnopqrstuvwxyz";
           }
            else if((exp[i+1]=='0')){
            str = true;
            lt = "0123456789";
           }
           i++;
        }
        NDFA ch;
        NDFA_state* st = fNdfa->add_state();
        (*st).stable = false;
        NDFA_state::line l;
        if(str){
            l.str = true;
            l.input = lt;
        }
        else{l.input.push_back(exp[i]);}
        NDFA_state* en = fNdfa->add_state();
        en->stable = false;
        l.to = en->id;
        st->trans.push_back(l);
        ch.start = st->id;
        ch.last = en->id;
        temp.push_back(ch);
      }


    if(temp.size()==2 && exp[i+1]!='+' && exp[i+1]!='*'){
          NDFA curr = temp.back();
          temp.pop_back();
          NDFA prev = temp.back();
          NDFA_state* en = fNdfa->get_state(prev.last);
          NDFA_state::line ltemp1;
          ltemp1.input = " ";
          ltemp1.to = curr.start;
          en->trans.push_back(ltemp1);
          temp.back().last = curr.last;
     }
   }

   if(ors.size()>0){
    ors.push_back(temp.back());
    temp.pop_back();
    temp.push_back(combine(ors,fNdfa));
   }

NDFA* prev = &temp.back();
return *prev;

}


NDFA NDFA::get_from_non_expression(string exp,Final_NDFA* fNdfa){

   NDFA_state* st = fNdfa->add_state();
   (*st).stable = false;
   NDFA_state::line l;
   l.input.push_back(exp[0]);
   NDFA_state* en = fNdfa->add_state();;
   en->stable = false;
   l.to = en->id;
   st->trans.push_back(l);
   this->start = st->id;
   this->last = en->id;

  for(int i=1;i<exp.length();i++){
    NDFA_state* temp = fNdfa->add_state();
    temp->stable = false;
    NDFA_state::line ltemp;
    ltemp.input.push_back(exp[i]);
    ltemp.to = temp->id;
    NDFA_state* prev = fNdfa->get_state(this->last);
    prev->trans.push_back(ltemp);
    this->last = temp->id;
  }

    fNdfa->get_state(this->last)->stable = true;
    fNdfa->get_state(this->last)->name = exp;
  return *this;
}


NDFA NDFA::combine(list<NDFA> states,Final_NDFA* fNdfa){

    NDFA fin;
    NDFA_state* newSt = fNdfa->add_state();
    newSt->stable = false;
    NDFA_state* newen = fNdfa->add_state();
    newen->stable = false;

    list<NDFA>::iterator it;
    for(it=states.begin();it!=states.end();it++){
    NDFA prev = *it;
    NDFA_state::line ltemp1;
    ltemp1.input = " ";
    ltemp1.to = prev.start;
    newSt->trans.push_back(ltemp1);

     NDFA_state::line ltemp2;
     ltemp2.input = " ";
     ltemp2.to = newen->id;
     NDFA_state* en = fNdfa->get_state(prev.last);
     en->trans.push_back(ltemp2);
    }

    fin.start = newSt->id;
    fin.last = newen->id;
    return fin;
}

