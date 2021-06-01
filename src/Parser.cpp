#include "Parser.h"
#include <iostream>
#include <fstream>
using std::fstream;
using std::cerr;

Parser::parse(){

    if(getNext){
        bool t = get_next_token();
        if (!t){
            if(st.empty()){
                return 0;
            }
            else{
                output.push_back("Error : Failed to match");
                return 0;
            }
        }
        getNext = false;
   }

   if(st.empty()){
         bool t = get_next_token();
         if(!t){return 0;}

         while(get_next_token()){
            string t = "Error : discard ";
             t += g->get_terminal(input);
             output.push_back(t);
         }
         return 0;
   }

   //output
   string ot ="";
   stack <pair<int,bool>>temp = st;  // pair <id , is terminal>
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
       else if(st.top().first == 0){st.pop();}  // if epsilon , pop
       else{
         string t = "Error : Missing ";
         t += g->get_terminal(st.top().first);
         t += " , inserted";
         output.push_back(t);
         st.pop();
       }
   }
   else{
      list<list<int>> ::iterator out = table.begin();
      advance(out,st.top().first);
      list<int>::iterator in = (*out).begin();
      if(input == -1){advance(in,g->terminals.size());}
      else{advance(in,input);}
      if((*in) == -2){  // sync
            string t ="Error : pop (";
            t += g->get_non_terminal(st.top().first)->name;
            output.push_back(t);
            st.pop();
      }
      else if ((*in) == -3){   // empty
        string t = "Error : illegal(";
         t += g->get_non_terminal(st.top().first)->name;
         t += " ) - discard ";
         t += g->get_terminal(input);
         output.push_back(t);

        getNext = true;
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

    /*
    //without linking to lexical
    if(file.empty()){return false;}
    string temp = file.front();
    file.pop_front();
    */

    //with linking to lexical

    string temp =linker.get_next_token();
    if(temp == ""){return false;}


    //convert string to id
    if(temp == "$"){input = -1;}
    else{
        input = g->get_terminal_id(temp);
    }
    return true;

}


Parser::writeFile(){
  fstream file;
  string text;
  file.open("Parser Output.txt",ios::out);
  if(!file){
        cerr << "Error: file could not be created" << endl;
        exit(1);
    }
  list<string>::iterator it;
  for(it=output.begin();it!=output.end();it++){
    file<< *it<<endl;
  }
  file.close();
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
