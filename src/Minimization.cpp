#include "Minimization.h"
#include <iostream>
#include <fstream>
using std::fstream;
using std::cerr;

Minimization::minimize(DFA dfa)
{
    list<DFA_state>::iterator it;
    Minimization::group nonst;
    list<int> ist;
    list<int> inonst;
    for(it=dfa.allStates.begin();it!=dfa.allStates.end();it++){
        if(it->stable){
             ist.push_back(it->id);
        }
        else{
            inonst.push_back(it->id);
        }
    }

    nonst.id = 0;
    num = 1;
    nonst.states = inonst;
    classes.push_back(nonst);

    list<list<int>> fst;
    list<int>::iterator stst;
    list<int> remain;
    list<int> newc;
    while(!ist.empty()){
    stst=ist.begin();
    remain.push_back(*stst);
    string n = dfa.get_state(*stst)->name;
    for(stst++;stst!=ist.end();stst++){
        if(dfa.get_state(*stst)->name!=n){
            newc.push_back(*stst);
        }
        else{remain.push_back(*stst);}
    }
    if(!remain.empty()){
    fst.push_back(remain);}
    ist.clear();
    ist.insert(ist.begin(),newc.begin(),newc.end());
    remain.clear();
    newc.clear();
   }


  list<list<int>>::iterator fit;
  for(fit=fst.begin();fit!=fst.end();fit++){
    Minimization::group st;
    st.id = num;
    num++;
    st.states = (*fit);
    classes.push_back(st);
  }
   list<Minimization::group>::iterator clit;
   for(clit=classes.begin();clit!=classes.end();clit++){
       mini((*clit).id,dfa);
    }


    for(clit=classes.begin();clit!=classes.end();clit++){
        DFA_state cl;
        cl.id = (*clit).id;
        list<DFA_state::line> lines = dfa.get_state((*clit).states.front())->trans;
        cl.trans.insert(cl.trans.end(),lines.begin(),lines.end());
        cl.stable = dfa.get_state((*clit).states.front())->stable ;
        if(cl.stable){cl.name = dfa.get_state((*clit).states.front())->name;}
        minimal.allStates.push_back(cl);
    }
    list<DFA_state>::iterator stt;
    for(stt=minimal.allStates.begin();stt!=minimal.allStates.end();stt++){
        list<DFA_state::line> newlines;
        list<DFA_state::line>::iterator l;
        for(l=(*stt).trans.begin();l!=(*stt).trans.end();l++){
            DFA_state::line temp;
            temp.input=(*l).input;
            temp.to = minimal.get_state(get_class_of_state(((*l).to)->id));
            if(contain(newlines,temp)){continue;}
            newlines.push_back(temp);
        }
        (*stt).trans.clear();
        (*stt).trans.insert((*stt).trans.begin(),newlines.begin(),newlines.end());
    }

    int start = minimal.allStates.front().id;
    int startClass = get_class_of_state(start);
    minimal.start = startClass;

    print_table(dfa);
    write_to_file();
}


Minimization::mini(int id,DFA dfa){
    bool changed = false;
    Minimization::group* cl = get_by_id(id);
    list<int>::iterator it;
    list<DFA_state::line>::iterator lit;
    list<int> remain;
    list<Minimization::group> temp;
    it=cl->states.begin();
    list<DFA_state::line> cline = (dfa.get_state(*it))->trans;
    remain.push_back(*it);
    it++;
   for(it;it!=cl->states.end();it++){
      DFA_state* st = dfa.get_state(*it);
      if(lines_equal(cline,st->trans)){
          remain.push_back(*it);
            continue;}
      else{
        changed = true;
        list<Minimization::group>::iterator clit;
         bool found = false;
      for(clit=temp.begin();clit!=temp.end();clit++){
          DFA_state* frist = dfa.get_state((*clit).states.front());
          if(lines_equal(frist->trans,st->trans)){
            get_by_id((*clit).id)->states.push_back(*it);
           found = true;
          }
      }
       if(!found){
         Minimization::group newcl;
         newcl.id = num;
         num++;
         newcl.states.push_back(*it);
         temp.push_back(newcl);
         classes.push_back(newcl);
       }
      }
   }
   if(!changed){return 0;}
   cl->states.clear();
   cl->states.insert(cl->states.end(),remain.begin(),remain.end());
   list<Minimization::group>::iterator clit;
      for(clit=classes.begin();clit!=classes.end();clit++){
        if((*clit).states.size() == 1){continue;}
        mini((*clit).id,dfa);
      }
}


int Minimization::get_class_of_state(int id){
 list<Minimization::group>::iterator it;
 for(it=classes.begin();it!=classes.end();it++){
   list<int>::iterator iit;
   for(iit=(*it).states.begin();iit!=(*it).states.end();iit++){
       if((*iit)==id){
        return (*it).id;
       }
   }
 }
}


bool Minimization::lines_equal(list<DFA_state::line> t1,list<DFA_state::line> t2){
   bool equ = true;
   list<DFA_state::line>::iterator lit1;
   list<DFA_state::line>::iterator lit2;
   list<DFA_state::line> temp;
   temp.insert(temp.end(),t2.begin(),t2.end());
   list<DFA_state::line> temp2;
   for(lit1=t1.begin();lit1!=t1.end();lit1++){
     DFA_state::line l1 = *lit1;
     bool found = false;
      for(lit2=temp.begin();lit2!=temp.end();lit2++){
        if((*lit2).input == l1.input){
            int s1 = get_class_of_state(((*lit2).to)->id);
            int s2 = get_class_of_state((l1.to)->id);
            if(s1==s2){
            found = true;
           }
        }
        else{
            temp2.push_back(*lit2);
        }
      }
      if(!found){
        equ = false;
        return equ;
      }

      temp.clear();
      if(!temp2.empty()){
      temp.insert(temp.end(),temp2.begin(),temp2.end());
      }
      temp2.clear();
   }
   if(!temp.empty()){
    equ = false;
   }

 return equ;
}


Minimization::group* Minimization::get_by_id(int id){
  list<Minimization::group>::iterator it;
  for(it=classes.begin();it!=classes.end();it++){
     if((*it).id==id){return &(*it);}
  }
}


Minimization::print_table(DFA dfa){

//print input
    list<string> validinputs;
    list<Minimization::group>::iterator cl;
    for(cl=classes.begin();cl!=classes.end();cl++){
        list<DFA_state :: line> ::iterator lineit;
		list<DFA_state::line> line = dfa.get_state((*cl).states.front())->trans;
		for(lineit=line.begin();lineit!=line.end();lineit++){
            list<string>::iterator insideIt;
            bool f = false;
           if(!validinputs.empty()){
              for(insideIt = validinputs.begin();insideIt!=validinputs.end();insideIt++){
                 if(((*lineit).input).compare(*insideIt)==0){f = true;}
              }}
           if(!f) {validinputs.push_back((*lineit).input);}
		}
    }

	list<string> printedInputs;
	list<string>::iterator sit;
	for(sit=validinputs.begin();sit!=validinputs.end();sit++){
            string temp = *sit;
        if(temp.size() == 1){
            printedInputs.push_back(temp);
        }
        else if(temp.size() == 26){
                if(temp[0]=='a'){
                 printedInputs.push_back("a->z");
                }
                else{
                    printedInputs.push_back("A->Z");
                }
        }
        else if((temp.size() == 10)&&(temp[0]=='0')){
           printedInputs.push_back("0->9");
        }
       else{
          string value1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
          string value2 = "abcdefghijklmnopqrstuvwxyz";
          string value3 = "0123456789";
          string org;
          string minusst;
            int tt=0;
            int enter = false;
            for(int i=0;i<value1.size();i++){
                if(temp.find(value1[i]) == std::string::npos){
                    tt++;
                    minusst.push_back(value1[i]);
                    minusst.push_back(',');
                    minusst.push_back(' ');
                }
            }
            if(tt==26){
                    minusst.clear();
                    tt=0;
                }
            else{
                enter = true;
                 org = "(A->Z)-{";
                 }

            if(!enter){
                 for(int i=0;i<value2.size();i++){
                if(temp.find(value2[i]) == std::string::npos){
                    tt++;
                    minusst.push_back(value2[i]);
                    minusst.push_back(',');
                    minusst.push_back(' ');
                }
               }
               if(tt==26){
                 minusst.clear();
                    tt=0;
               }
               else{
                enter = true;
                 org = "(a->z)-{";
                 }
            }

            if(!enter){
              for(int i=0;i<value3.size();i++){
                if(temp.find(value3[i]) == std::string::npos){
                    tt++;
                    minusst.push_back(value3[i]);
                    minusst.push_back(',');
                    minusst.push_back(' ');
                }
             }
             if(tt==10){
                minusst.clear();
                    tt=0;
             }
             else{
                enter = true;
                 org = "(0->9)-{";
             }
            }
            minusst.pop_back();
            minusst.pop_back();
            minusst.push_back('}');
          org.insert(org.end(),minusst.begin(),minusst.end());
          printedInputs.push_back(org);
       }
	}

	string inp ;

//print classes
  list<DFA_state>::iterator stit;
  int s = minimal.allStates.size();
  for(int i=0;i<s;i++){
    DFA_state* tpst = minimal.get_state(i);
     list<DFA_state :: line> ::iterator lt;
     list<DFA_state::line> linet = tpst->trans;
     //if(linet.empty()){continue;}
    inp = "State:\t";
     inp+= to_string(tpst->id);
     inp+="\t\t";
     bool stable = tpst->stable;
     if(stable){
        inp+="(acceptance state: \"";
        inp+= tpst->name;
        inp+= "\")";
     }
     inp+="\n";
     if(linet.empty()){
         inp+="-----------------------------------------------------------------------";
         table.push_back(inp);
         inp="";
         continue;}
     for(lt=linet.begin();lt!=linet.end();lt++){
        string h = (*lt).input;
        list<string>::iterator sit2,sit1;
        inp+="\'";
        for(sit2=validinputs.begin(),sit1=printedInputs.begin();sit2!=validinputs.end(),sit2!=printedInputs.end();sit2++,sit1++){
            if((*sit2)==h){
                inp+=(*sit1);
                break;
            }
        }
        inp+="\' ";
        inp+="-->";
        inp+=to_string(((*lt).to)->id);
        inp+="\t";
     }
     inp+="\n";
     inp+="-----------------------------------------------------------------------";
     table.push_back(inp);
     inp="";
  }

}



Minimization::write_to_file()
{
  fstream file;
  string text;
  file.open("Table.txt",ios::out);
  if(!file){
        cerr << "Error: file could not be created" << endl;
        exit(1);
    }
  list<string>::iterator it;
  for(it=table.begin();it!=table.end();it++){
    file<< *it<<endl;
  }
  file.close();
}


bool Minimization::contain(list<DFA_state::line> lst,DFA_state::line l){
    bool found = false;
    string in = l.input;
    int to = (l.to)->id;
   list<DFA_state::line>::iterator it;
   for(it=lst.begin();it!=lst.end();it++){
       if(((*it).input==in)&&(((*it).to)->id == to)){
        found = true;
        return found;
       }
   }
   return found;
}
