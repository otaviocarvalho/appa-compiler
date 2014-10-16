#include <stdio.h>
#include <stdlib.h>
#include "gv.h"
#include "comp_dict.h"
#include "iks_ast.h"

typedef struct comp_list_t {
    comp_dict_item_t* item;
    struct comp_list_t* next;
} comp_list_t;

typedef struct comp_tree_t {
    int id;
    int type;
    char* lex;
    comp_dict_item_t* hash;
    comp_list_t* list_args;
    struct comp_tree_t* children;
    struct comp_tree_t* next_brother;
    int tipo_coersao;
    int valor_coersao;
} comp_tree_t;

comp_tree_t* arvore_sintatica;

comp_tree_t* create_node(int, char*, comp_tree_t*, comp_dict_item_t*);
comp_tree_t* create_empty_node();
void free_syntax_tree(comp_tree_t*);
void print_syntax_tree(comp_tree_t*);

void verifica_funcao(comp_tree_t*, char*);
void verifica_output(comp_tree_t* node);
void verifica_input(comp_tree_t* node);
void verifica_return(comp_tree_t*, char*, int);
int encontra_tipo(char*, int);
void verifica_atribuicao(comp_tree_t*, int);
int encontra_operador(char*);
void verifica_tipo_indexador(comp_tree_t* );
void verifica_argumentos(comp_tree_t*, char*, comp_list_t*);
comp_dict_item_t* encontra_item_operador(char*, int);

void list_func_connect(comp_tree_t*, comp_list_t*, comp_dict_item_t*);
comp_list_t* list_concat(comp_list_t*, comp_list_t*);
comp_list_t* list_create(comp_dict_item_t*);
int list_count(comp_list_t*);
int verifica_coersao_arvore(comp_tree_t*);
