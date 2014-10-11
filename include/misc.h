#ifndef __MISC_H
#define __MISC_H

#include <stdio.h>
#include "gv.h"
#include "comp_dict.h"

extern int cur_line;
extern comp_stack_dict_t* stack_scope; // Ponteiro para a pilha de escopos
extern struct comp_dict_t* symbol_table_root; // Ponteiro para a raiz da tabela de símbolos
extern struct comp_dict_t* symbol_table_cur; // Ponteiro para a tabela de símbolos do escopo corrente
extern struct comp_tree_t* arvore_sintatica;

int getLineNumber (void);
void yyerror (char const *mensagem);
void main_init (int argc, char **argv);
void main_finalize (void);

#endif
