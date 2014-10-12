//#include "comp_dict.h"
#include <stdio.h>
#include <stdlib.h>
#include "gv.h"
#include "comp_dict.h"

typedef struct comp_tree_t {
    int id;
    int type;
    char* lex;
    comp_dict_item_t* hash;
    struct comp_tree_t* children;
    struct comp_tree_t* next_brother;
} comp_tree_t;

comp_tree_t* arvore_sintatica;
extern struct comp_dict_item_t* last_func_call;

comp_tree_t* create_node(int, char*, comp_tree_t*, comp_dict_item_t*);
comp_tree_t* create_empty_node();
void free_syntax_tree(comp_tree_t*);
void print_syntax_tree(comp_tree_t*);

void verifica_output(comp_tree_t* node);
void verifica_input(comp_tree_t* node);


