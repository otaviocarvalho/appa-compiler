#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>

#define SIZE_TABLE_KEY 100
#define PRIME 211 //211 // 9901
#define EOS '\0'

typedef struct comp_dict_item_t {
    int line;
} comp_dict_item_t;

typedef struct comp_dict_node_t {
    char* key;
    comp_dict_item_t* item;
    struct comp_dict_node_t *next;
} comp_dict_node_t;

typedef struct comp_dict_t {
    comp_dict_node_t* entries[PRIME];
} comp_dict_t;

// Diretivas externas
extern char* yytext;

// Funções
comp_dict_t* init_table_tree();
int free_table_tree();
int init_dict(comp_dict_t*);
int free_dict(comp_dict_t*);
int hash_function(char*);
comp_dict_item_t* find_symbol(comp_dict_t* cur_table, char* key);
int add_symbol(comp_dict_t* cur_table, char* key, int line);
int print_table(comp_dict_t*);
char* str_entry(char*, char*, int);
int print_file_table(FILE*, comp_dict_t*);
