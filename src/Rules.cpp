#include "Rules.h"

#include <iostream>
#include <fstream>
using std::fstream;
using std::cerr;

Rules::read_from_file()
{
    ifstream file;
    string text;
    file.open(path);
    if(!file){
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    while(getline(file,text)){
        rule.push_back(text);
    }

    file.close();
}


Rules::convert_to_expressions()
{
    list<string> ::iterator it;
    for(it=rule.begin();it!=rule.end();it++){
        string temp = *it;
        for(int i=0;i<temp.length();i++){

          if(temp[i]==' '){
            continue;
          }

        //keywords and punctuations
          if(temp[i]=='{'||temp[i]=='['){
            bool key = (temp[i]=='{');
            string keyword;
            for(int j=i+1;j<temp.length();j++){
                if((temp[j]=='}'&&key)||(temp[j]==']'&&temp[j-1]!='\\'&&!key)){
                  if(keyword!=""&&keyword!=" "&&key){
                        expressions.push_back(Exp_Node("keyword",keyword,false));
                  }
                  else if(keyword!=""&&keyword!=" "&&!key){
                        expressions.push_back(Exp_Node("punctuations",keyword,false));
                  }
                    keyword = "";
                    i=j;
                    break;
                }
                else if(temp[j]==' '){
                   if(keyword!=""&&keyword!=" "&&key){
                        expressions.push_back(Exp_Node("keyword",keyword,false));
                   }
                  else if(keyword!=""&&keyword!=" "&&!key){
                        expressions.push_back(Exp_Node("punctuations",keyword,false));
                  }
                   keyword = "";
                }
                else if(temp[j]=='\\'){
                   continue;
                }
                else{
                  keyword.push_back(temp[j]);
                }
                i=j;
            }
          }

          //definitions and expressions
         else{
            bool def;
            string name;
            for(int j=i;j<temp.length();j++){
                if(temp[j]==':'){
                   def = false;
                    i=j;
                   break;
                }
                else if(temp[j]=='='){
                   def = true;
                    i=j;
                   break;
                }
                if(temp[j]==' '){
                   continue;
                }
                else{
                  name.push_back(temp[j]);
                }
                i=j;
            }
            string value;
            for(int j=i+1;j<temp.length();j++){
                if(temp[j]=='\\'||temp[j]==' '){
                    if(temp[j]=='\\'&&(temp[j+1]=='L'||temp[j+1]=='+'||temp[j+1]=='*')){
                       value.push_back(temp[j]);
                    }
                   continue;
                }
                else{
                  value.push_back(temp[j]);
                }
                i=j;
            }
           if(def){Exp_Node tnode = Exp_Node(name,value,false);
                definitions.push_back(tnode);}
           else{Exp_Node tnode = Exp_Node(name,value,true);
               expressions.push_back(tnode);
               priority.push_back(name);}

         }

        }
    }
    if(!definitions.empty()){
        resolve_definitions();
    }

    list<Exp_Node>::iterator exitt;
    list<string> key;
    for(exitt=expressions.begin();exitt!=expressions.end();exitt++){
        if(!((*exitt).exp)){
            key.push_back((*exitt).value);
        }
    }
    if(!key.empty()){
        priority.insert(priority.begin(),key.begin(),key.end());
    }

}


Rules::resolve_definitions()
{
  list<Exp_Node> ::iterator itt;

  //replace definitions in other definitions
  for(itt=definitions.begin();itt!=definitions.end();itt++){
        Exp_Node &t = *itt;
        string value = t.value;
        list<Exp_Node> ::iterator itd;
        for(itd=definitions.begin();itd!=definitions.end();itd++){
         Exp_Node td = *itd;
         if(td.name==t.name){continue;}
         string valued = td.name;
         for(int pos = value.find(valued);pos!= std::string::npos;pos = value.find(valued)){
            string temp = td.value;
            value.replace(pos,valued.length(),temp);
            t.value = value;
        }
        }
  }


  //replace definitions in expressions
  list<Exp_Node> ::iterator it;
  for(it=expressions.begin();it!=expressions.end();it++){
        Exp_Node &t = *it;
        if(!t.exp){continue;}
        string value = t.value;
        list<Exp_Node> ::iterator itd;
        for(itd=definitions.begin();itd!=definitions.end();itd++){
         Exp_Node td = *itd;
         string valued = td.name;
         string value2 = value;
         for(int pos = value.find(valued);pos!= std::string::npos;pos = value2.find(valued)){
            string temp = "("+td.value+")";
            if((pos+valued.length()<value.length())&&(isalpha(value[pos+valued.length()]))){
                    value2.replace(pos,valued.length(),temp);
                    continue;}
            value2.replace(pos,valued.length(),temp);
            value.replace(pos,valued.length(),temp);
            t.value = value;
        }


        string valued1 = "a-z";
        string valued2 = "A-Z";
        string valued3 = "0-9";
         for(int pos = value.find(valued1);pos!= std::string::npos;pos = value.find(valued1)){
            string rep1 = "\\a";
            value.replace(pos,valued1.length(),rep1);
            t.value = value;
        }

        for(int pos = value.find(valued2);pos!= std::string::npos;pos = value.find(valued2)){
            string rep2 = "\\A";
            value.replace(pos,valued2.length(),rep2);
            t.value = value;
        }

        for(int pos = value.find(valued3);pos!= std::string::npos;pos = value.find(valued3)){
            string rep3 = "\\0";
            value.replace(pos,valued3.length(),rep3);
            t.value = value;
        }

        }
  }

}

string Rules::get_priority(list<string> names){
    list<string>::iterator it;
    list<string>::iterator it2;
    for(it2=priority.begin();it2!=priority.end();it2++){
      for(it=names.begin();it!=names.end();it++){
         if((*it)==(*it2)){return *it;}
      }
   }
}
