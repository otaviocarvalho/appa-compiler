#ifndef __COMP_DICT_H
#define __COMP_DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>
#include <main.h>
#include "definitions.h"

#define SIZE_TABLE_KEY 100
#define PRIME 211 //211 // 9901
#define EOS '\0'

typedef struct comp_dict_item_t {
    int operador;
    int line;
    int type_var;   // IKS_TYPE_NOT_DEFINED / IKS_INT / IKS_FLOAT / IKS_BOOL / IKS_CHAR / IKS_STRING
    int type;       // IKS_SIMBOLO_...
    void* value;
    char* key;
    int count_args;
    struct comp_list_t* list_args;
    int desloc;
    int escopo;
} comp_dict_item_t;

typedef struct comp_dict_node_t {
    char* key;
    int tipo_coersao;
    comp_dict_item_t* item;
    struct comp_dict_node_t *next;
} comp_dict_node_t;

typedef struct comp_dict_t {
    int id;
    int desloc;
    comp_dict_node_t* entries[PRIME];
} comp_dict_t;

typedef struct comp_stack_dict_t {
    struct comp_dict_t *dict;
    struct comp_stack_dict_t* next;
} comp_stack_dict_t;

// Diretivas externas
extern comp_stack_dict_t* stack_scope; // Ponteiro para a pilha de escopos
extern struct comp_dict_t* symbol_table_root; // Ponteiro para a raiz da tabela de símbolos
extern struct comp_dict_t* symbol_table_cur; // Ponteiro para a tabela de símbolos do escopo corrente

// Funções
comp_dict_t* init_table_tree();
int free_table_tree();
int init_dict(comp_dict_t*);
int free_dict(comp_dict_t*);
int hash_function(char*);
comp_dict_item_t* find_symbol(comp_dict_t*, char*);
comp_dict_item_t* add_symbol(comp_dict_t*, char*, int, int, int, int, int);
void* alloc_value_symbol(int, char*);
int print_table(comp_dict_t*);
char* str_entry(char*, char*, int, int, int ,void*, int);
int print_file_table(FILE*, comp_dict_t*);

comp_dict_t* create_table(int);
comp_stack_dict_t* stack_dict_init();
comp_stack_dict_t* stack_dict_push(comp_stack_dict_t*, comp_dict_t*);
comp_stack_dict_t* stack_dict_pop(comp_stack_dict_t*);
void print_stack_dict(comp_stack_dict_t*);
int verifica_se_existe(comp_dict_t*, char*, int, int, int);
int verifica_uso(int, int, char*);
#endif
