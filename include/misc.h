#ifndef __MISC_H
#define __MISC_H
#include <stdio.h>
#include <search.h>
#include "comp_dict.h"

#define HASH_SIZE 1000000

int cur_line = 1;   // Inicializa o compilador na linha 1

struct comp_dict_tree_t* symbol_table_root; // Ponteiro para a raiz da tabela de símbolos
struct comp_dict_tree_t* symbol_table_cur; // Ponteiro para a tabela de símbolos do escopo corrente

int getLineNumber (void);
void yyerror (char const *mensagem);
void main_init (int argc, char **argv);
void main_finalize (void);

#endif
