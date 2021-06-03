#include "Link.h"
#include <iostream>

Link::Link()
{
    rules.path="Rules.txt";
    rules.read_from_file();
    rules.convert_to_expressions();

    ndfa.construct_NDFA(rules.expressions);

    dfa.construct_DFA(ndfa,rules);

    mini.minimize(dfa);

    tk.read_from_file("Input.txt");
    lexical.copy_to(tk);

}


string Link::get_next_token(){
  string temp = lexical.get_next_token(mini.minimal);
  tk.output.push_back(temp);
  if(temp == "assign"){temp = "=";}
  return temp;
}
