#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>

#define SIZE_TABLE_KEY 110

typedef struct comp_dict_item_t comp_dict_item_t;
struct comp_dict_item_t {
    int line;
};

typedef struct comp_dict_t comp_dict_t;
struct comp_dict_t {
    char key[SIZE_TABLE_KEY];
    comp_dict_item_t item;
    comp_dict_t *next;
};

typedef struct comp_dict_tree_t {
    int id;
    struct comp_dict_tree_t *next_brother;
    struct comp_dict_tree_t *sons;
} comp_dict_tree_t ;

// Diretivas externas
extern char* yytext;

// Funções
comp_dict_tree_t* init_table_tree();
void free_table_tree(comp_dict_tree_t* tree);
comp_dict_t* add_symbol(comp_dict_tree_t* cur_table, char* key, int line);
comp_dict_t* find_symbol(comp_dict_tree_t* cur_table, char* key);
void print_symbol(comp_dict_tree_t* cur_table);
