#include "misc.h"

int getLineNumber (void){
    return cur_line;
}

void yyerror (char const *mensagem){
    fprintf (stderr, "%s (line %d)\n", mensagem, cur_line);
}

void main_init (int argc, char **argv){
    symbol_table_root = init_table_tree();
    symbol_table_cur = symbol_table_root;
}

void main_finalize (void){
    free_table_tree(symbol_table_root);
    /*free_syntax_tree(arvore_sintatica);*/
}
