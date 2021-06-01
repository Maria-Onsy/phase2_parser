#include "Parser.h"

Parser::parse(){

    if(getNext){
        bool t = get_next_token();
        if (!t){
            if(st.empty()){
                return 0;
            }
            else{
                error_recovery();
                parse();
            }
        }
        getNext = false;
   }

   if(st.empty()){
        error_recovery();
        parse();
   }

   //output
   string ot ="";
   stack <pair<int,bool>>temp = st;
   int y = st.size()-1;
   for(int i=0;i<y;i++){
    if(temp.top().second){
        ot+= g->get_terminal(temp.top().first);
        ot+= " ";
    }
    else{
        ot+= g->get_non_terminal(temp.top().first)->name;
        ot+= " ";
    }
    temp.pop();
   }
   output.push_back(ot);


   if(st.top().second){
       if(st.top().first == input){
         // add to output list
       /*  int temp = outIndx.top();
         if(output.size()<=temp){output.push_back(g->get_terminal(st.top().first));}
         else{
           list<string>::iterator t = output.begin();
           advance(t,temp);
           (*t) += g->get_terminal(st.top().first);
         }
         outIndx.pop();
       */
         st.pop();
         getNext = true;
       }
       else if(st.top().first == 0){st.pop();}
       else{
        error_recovery();
       }
   }
   else{
      list<list<int>> ::iterator out = table.begin();
      advance(out,st.top().first);
      list<int>::iterator in = (*out).begin();
      if(input == -1){advance(in,g->terminals.size());}
      else{advance(in,input);}
      if((*in)== -2 || (*in)==-3){ //error or sync
        error_recovery();
      }
      else{
         rule* r = g->get_rule((*in));

         //add to output
       /* int temp = outIndx.top();
         if(output.empty()||output.size()<=temp){output.push_back(g->get_non_terminal(st.top().first)->name);}
         else{
           list<string>::iterator t = output.begin();
           advance(t,temp);
           (*t) += g->get_non_terminal(st.top().first)->name;
         }
         outIndx.pop();
        */
         st.pop();

         list<Node>::iterator it;
         stack<pair<int,bool>> rev;
         for(it=r->to.begin();it!=r->to.end();it++){
            if((*it).terminal){
                rev.push(pair<int,bool>(g->get_terminal_id((*it).name),true));
          //      outIndx.push(temp+1);

            }
            else{
                rev.push(pair<int,bool>((g->get_non_terminal((*it).name))->id,false));
            //     outIndx.push(temp+1);
            }
         }
         int j = rev.size();
         for(int i=0;i<j;i++){
                st.push(rev.top());
                rev.pop();
         }
      }
   }

   parse();

}

bool Parser::get_next_token(){

    //without linking to lexical
    if(file.empty()){return false;}
    string temp = file.front();
    file.pop_front();

    //convert string to id
    if(temp == "$"){input = -1;}
    else{
        input = g->get_terminal_id(temp);
    }
    return true;

}

Parser::error_recovery(){

}

//To test without linking to lexical
Parser::add_input(){
  file.push_back("int");
  file.push_back("id");
  file.push_back(";");
  file.push_back("id");
  file.push_back("=");
  file.push_back("num");
  file.push_back(";");
  file.push_back("if");
  file.push_back("(");
  file.push_back("id");
  file.push_back("relop");
  file.push_back("num");
  file.push_back(")");
  file.push_back("{");
  file.push_back("id");
  file.push_back("=");
  file.push_back("num");
  file.push_back(";");
  file.push_back("}");
  file.push_back("else");
  file.push_back("{");
  file.push_back("id");
  file.push_back("=");
  file.push_back("num");
  file.push_back(";");
  file.push_back("}");
  file.push_back("$");

}
