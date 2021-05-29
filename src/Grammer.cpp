#include "Grammer.h"
#include <iostream>
#include <fstream>
using std::fstream;
using std::cerr;

Grammer::get_from_file()
{
    //scan from file
    string input = "";
    ifstream file;
    string text;
    file.open(file_name);
    if(!file){
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    while(getline(file,text)){
        input += text;
    }
    file.close();



    for(int i=0;i < input.size();i++){
        if(input[i] == '#'){

            //non_terminal name
            string non_term="";
            i++;
            while(input[i] == ' '){
                i++;
                continue;}
            while((input[i] != ' ')&&(input[i] != '=')){
                non_term +=input[i];
                i++;
            }


            // "="
            if(input[i] == '='){i++;}
            else{
                while(input[i] == ' '){
                i++;
                continue;}
                if(input[i] == '='){i++;}
            }
            while(input[i] == ' '){
                i++;
                continue;}

            list<rule> rules;
            //rule
            while((i< input.size())&&(input[i] != '#')){

                list<Node> nodes;

                while(input[i] == ' '){
                i++;
                continue;}

                while(i<input.size()){

                    while((i<input.size())&&(input[i] == ' ')){
                     i++;
                     continue;}

                   bool term = false;
                   if(input[i] == '\''){
                      term = true;
                      i++;
                   }

                   string rname="";
                   while((i<input.size()&&input[i]!='#')&&((input[i] != ' ')&&(input[i] != '\'')&&(input[i] != '|'))){
                     rname+=input[i];
                     i++;
                   }

                   if(term&& !termContain(rname)){
                    terminals.push_back(rname);
                   }
                   nodes.push_back(Node(rname,term));

                   if(i>=input.size() || input[i]== '#'){break;}
                   else if(term&&input[i] == '\''){i++;}
                   if(i>=input.size()|| input[i]== '#'){break;}
                   while((i<input.size())&&(input[i] == ' ')){
                     i++;
                     continue;}
                    if(input[i] == '|'){i++;break;}

                }

                rule r= rule(numRule, numNonterm, nodes);
                numRule++;
                rules.push_back(r);
                allRules.push_back(r);

            }

            non_terminals.push_back(Non_terminal(numNonterm,non_term,rules));
            numNonterm++;
            i--;
        }
    }

}

Non_terminal* Grammer:: get_non_terminal(int id){
  list<Non_terminal>::iterator it;
  for(it=this->non_terminals.begin();it!=this->non_terminals.end();it++){
    if((*it).id == id){
        return &(*it);
    }
  }
}

Non_terminal* Grammer:: get_non_terminal(string name){
  list<Non_terminal>::iterator it;
  for(it=this->non_terminals.begin();it!=this->non_terminals.end();it++){
    if((*it).name == name){
        return &(*it);
    }
  }
}

 rule* Grammer::get_rule(int id){
   list<rule>::iterator it;
   for(it=this->allRules.begin();it!=this->allRules.end();it++){
     if((*it).id == id){
         return &(*it);
     }
  }
 }

 int Grammer:: get_terminal_id(string name){
  list<string>::iterator it;
  int i=0;
  for(it=this->terminals.begin();it!=this->terminals.end();it++){
    if((*it) == name){
        return i;
    }
    i++;
  }
}

 bool Grammer::termContain(string name){
     bool found = false;
     list<string>::iterator it;
     for(it=this->terminals.begin();it!=this->terminals.end();it++){
     if((*it) == name){
         found = true;
         return found;
     }
  }
  return found;
 }
