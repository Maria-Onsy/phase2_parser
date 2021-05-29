#include "Parser_Table.h"

Parser_Table::Parser_Table(Grammer g){
  grammer = &g;
  nonLL1 = false;
  get_frist();
  get_follow();
  construct_table();
}

Parser_Table::get_frist(){

}

Parser_Table::get_follow(){

}

Parser_Table::construct_table(){

}

