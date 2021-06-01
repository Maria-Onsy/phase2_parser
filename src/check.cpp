#include "check.h"
#include "Tokens.h"
#include "DFA_state.h"

#include <string.h>
#include <list>
using namespace std;
#include <iostream>

check::Report_Error(int z){
 bool flag = false;
 list<int> error;
 list<int>::iterator i;
 for(i = error.begin(); i!=error.end(); i++){
  int t = *i;
  if(t == z){flag = false;}
 }
 if(flag){error.push_back(z); cout << "There is an error in line" << z << endl;}
}


check::Check(Tokens token,DFA df)
{

int z=0;
list<string> l = token.input;
list<string>:: iterator it;
for(it = l.begin(); it!=l.end(); it++){
 z++;
 bool flag = true;
 string st = *it;
 string accepted = "no";      //to indicate if there is an accepted token (last accepted token)
 string s = "";
 int stop = 0;                //to keep track of index of last accepted token
 DFA_state* state = df.get_state(df.start);

 for(int i=0;i<st.length();i++){
  if(st[i]==' '){continue;}
  list<DFA_state::line> q = state->trans;
  list<DFA_state::line>:: iterator k;
  for(k = q.begin(); k!=q.end(); k++){
   if((*k).input.find(st[i]) != std::string::npos){                    //input is still have path in list
    state = (*k).to;
    s = s + st[i];
    if(state->stable){ accepted = state->name; stop = i;}           //Needed
        break;
   }
  }
  if(st[i+1] == ' ' || i == st.length()-1 ){
    if( accepted != "no" ){                         //input have no path and there is an acceptance state
        if ( accepted == "id" ){                    // if it was an id add it to id table
            list<node>::iterator j;
            for(j = ids.begin(); j!=ids.end(); j++){
                node t = *j;
                if(t.name == s){flag = false;}
            }
            if(flag){node n; n.name = s; ids.push_back(n);}
        }
        token.output.push_back(accepted);       //add it to output
        i = stop; s=""; accepted = "no"; state = df.get_state(df.start); continue;
    }
    else{Report_Error(z); i = stop+1; s=""; accepted = "no"; state = df.get_state(df.start); continue;}       //input have no path and no acceptance state, remove char and start again
 }
 }
}

token.write_to_file();
}

check::copy_to(Tokens token){
  list<string>:: iterator it;
  for(it = token.input.begin(); it!=token.input.end(); it++){
     in += (*it);
     in += " ";
   }
  // state = df.get_state(df.start);
}

string check::get_next_token(DFA df){

    num++;
    bool flag = true;
    string accepted = "no";      //to indicate if there is an accepted token (last accepted token)
    string s = "";
    int stop = 0;                //to keep track of index of last accepted token
    DFA_state* state = df.get_state(df.start);

    if(num == in.length()-1){
            return "$";}
    else if(num>in.length()-1){
            return "";}

    for(int i=num;i<in.length();i++){
        if(in[i]==' '){continue;}
        list<DFA_state::line> q = state->trans;
        list<DFA_state::line>::iterator k;
        for(k = q.begin(); k!=q.end(); k++){
           if((*k).input.find(in[i]) != std::string::npos){                    //input is still have path in list
           state = (*k).to;
           s = s + in[i];
           if(state->stable){ accepted = state->name; stop = i;}           //Needed
           break;
   }
  }
  if(in[i+1] == ' ' || i == in.length()-1 ){
    if( accepted != "no" ){                         //input have no path and there is an acceptance state
        if ( accepted == "id" ){                    // if it was an id add it to id table
            list<node>::iterator j;
            for(j = ids.begin(); j!=ids.end(); j++){
                node t = *j;
                if(t.name == s){flag = false;}
            }
            if(flag){node n; n.name = s; ids.push_back(n);}
        }

        num = stop;
        return accepted;
    }
    else{Report_Error(num); num = stop+1; //show error
         return get_next_token(df);}       //input have no path and no acceptance state, remove char and start again
 }
 }
}


