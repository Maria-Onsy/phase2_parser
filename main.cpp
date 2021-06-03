#include <iostream>
#include "Grammar.h"
#include "Parser_Table.h"
#include "Parser.h"

using namespace std;

int main()
{
    Grammar g = Grammar("grammar.txt");
    g.get_from_file();

    Parser_Table table = Parser_Table(&g);

    Parser p = Parser(table.table,&g);

    if(table.notLL1){
        p.output.push_back(table.error_message);
        p.writeFile();
        return 0;
    }

    p.parse();
    p.writeFile();
    p.linker.tk.write_to_file();


    return 0;
}
