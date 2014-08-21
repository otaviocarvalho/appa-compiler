#include "comp_dict.h"
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>

// Inicia a árvore de tabelas
comp_dict_tree_t* init_table_tree(){
    comp_dict_tree_t* root = malloc(sizeof(comp_dict_tree_t));
    root->id = 1;
    root->sons = NULL;
    root->next_brother = NULL;

    return root;
}

// Libera o conteúdo alocado pelo ponteiro da raiz da árvore
void free_table_tree(comp_dict_tree_t* tree){
    free(tree);
}

// Adiciona um símbolo a uma tabela existente
comp_dict_t* add_symbol(comp_dict_tree_t* cur_table, char* key, int line){
    ENTRY e, *e_search;
    char *table_key = malloc(sizeof(char)*SIZE_TABLE_KEY);

    // Prepara entrada de dicionário a ser adicionada na tabela hash
    comp_dict_t* data = malloc(sizeof(comp_dict_t));
    strcpy(data->key, yytext);
    data->item.line = line;
    data->next = NULL;

    // Insere na tabela
    e.key = strcat(table_key, key);
    e.data = data;
    e_search = hsearch(e, ENTER);
    if (e_search == NULL) {
        fprintf(stderr, "Error: insert on table failed\n");
        exit(1);
    }

    return e_search->data;
}

// Procura um símbolo em uma tabela existente
comp_dict_t* find_symbol(comp_dict_tree_t* cur_table, char* key){
    ENTRY e, *ep;
    comp_dict_t* data = malloc(sizeof(comp_dict_t));

    e.key = key;
    ep = hsearch(e, FIND);

    if (ep != NULL){
        memcpy(data, ep->data, sizeof(comp_dict_t *));
        return ep->data;
    }

    return NULL;
}
