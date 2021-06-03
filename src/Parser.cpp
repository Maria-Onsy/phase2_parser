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
                return 0;  //both of the stack and the input are empty
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


   //if the top of the stack is terminal
   if(st.top().second){

        //top of the stack match the input
       if(st.top().first == input){
         st.pop();
         getNext = true;
       }

       //top of stack is epsilon
       else if(st.top().first == 0){st.pop();}  // if epsilon , pop

       //Mismatch
       else{
         string t = "Error : Missing ";
         t += g->get_terminal(st.top().first);
         t += " , inserted";
         output.push_back(t);
         st.pop();
       }
   }


   //top of the stack is nonterminal
   else{
      list<list<int>> ::iterator out = table.begin();
      advance(out,st.top().first);
      list<int>::iterator in = (*out).begin();
      if(input == -1){advance(in,g->terminals.size());}
      else{advance(in,input);}

      //sync
      if((*in) == -2){
            string t ="Error : pop (";
            t += g->get_non_terminal(st.top().first)->name;
            output.push_back(t);
            st.pop();
      }

      // error
      else if ((*in) == -3){
        string t = "Error : illegal(";
         t += g->get_non_terminal(st.top().first)->name;
         t += " ) - discard ";
         t += g->get_terminal(input);
         output.push_back(t);

        getNext = true;
      }


      else{
         rule* r = g->get_rule((*in));
         st.pop();

         list<Node>::iterator it;
         stack<pair<int,bool>> rev;
         for(it=r->to.begin();it!=r->to.end();it++){
            if((*it).terminal){
                rev.push(pair<int,bool>(g->get_terminal_id((*it).name),true));
            }
            else{
                rev.push(pair<int,bool>((g->get_non_terminal((*it).name))->id,false));
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

    string temp =linker.get_next_token();
    if(temp == ""){return false;}  //the input is empty

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

