#include "comp_dict.h"
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>

// Inicia a árvore de tabelas
comp_dict_t* init_table_tree(comp_dict_t *root){
    root = malloc(sizeof(comp_dict_t));
    init_dict(root);

    return root;
}

// Desaloca a árvore de tabelas
int free_table_tree(comp_dict_t *root){
    free_dict(root);
    free(root);

    return 0;
}

// Função para inicializar um dicionário
int init_dict(comp_dict_t* table){
    int i;

    for (i = 0; i < PRIME; i++){
        table->entries[i] = NULL;
    }

    return 0;
}

// Libera o conteúdo alocado para um dicionário
int free_dict(comp_dict_t* table){
    int i = 0;
    for (i = 0; i < PRIME; i++){
        if (table->entries[i] != NULL){
            comp_dict_node_t* current = table->entries[i];
            comp_dict_node_t* next = current->next;

            free(current->key);
            free(current->item);
            free(current);
            while (next != NULL){
                current = next;
                next = current->next;

                free(current->key);
                free(current->item);
                free(current);
            }

            table->entries[i] = NULL;
        }
    }

    return 0;
}

// Função que recebe uma chave e calcula o bucket usando o algoritmo PJW
int hash_function(char* key){
    char *p;
    unsigned h = 0, g;

    for (p = key; *p != EOS; p = p+1){
        h = (h << 4) + (*p);

        if (g = h&0xf0000000){
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }

    return h % PRIME;
}

// Procura um item em uma tabela existente
comp_dict_item_t* find_symbol(comp_dict_t* cur_table, char* key){
    int hash = hash_function(key); // Calcula o valor da tabela hash com base no nome

    // Verifica se existem entradas na tabela
    if (cur_table->entries[hash] != NULL){
        comp_dict_node_t* current = cur_table->entries[hash]; // Inicia com o primeiro nodo da lista encadeada

        // Percorre a lista até encontrar
        while ( (strcmp(key, current->key)) && (current->next != NULL) ){
            current = current->next; // vai para o nodo seguinte
        }

        // Retorna o ponteiro ao encontrar
        if ( !(strcmp(key, current->key)) ){
            return current->item;
        }
    }

    return NULL;
}

// Adiciona um item a uma tabela existente
comp_dict_item_t* add_symbol(comp_dict_t* cur_table, char* key, int line){
    // Aloca estruturas de dados para o novo nodo
    comp_dict_node_t* node = malloc(sizeof(comp_dict_node_t));
    node->item = malloc(sizeof(comp_dict_item_t));

    node->key = strdup(key);
    node->next = NULL;
    node->item->line = line;

    // Calcula o hash
    int hash = hash_function(key);
    // Inicializa bucket com esse elemento se não encontrar nenhum
    if (cur_table->entries[hash] == NULL) {
        cur_table->entries[hash] = node;
    }
    // Atualiza linha se já existe
    else if ( strcmp(key, cur_table->entries[hash]->key) == 0 ){
        cur_table->entries[hash]->item->line = line;
        free(node->key);
        free(node->item);
        free(node);
    }
    // Se não existir, adiciona elemento no fim da lista
    else {
        node->next = cur_table->entries[hash];
        cur_table->entries[hash] = node; // Elemento recém criado vira o primeiro da lista
    }

    return cur_table->entries[hash]->item;
}

// Varre a tabela de símbolos imprimindo o conteúdo
int print_file_table(FILE* out, comp_dict_t* table) {
    int entry_count = 0;
    int i = 0;

    for (i = 0; i < PRIME; i++){
        if (table->entries[i] != NULL){
            char tmp_string[500];

            comp_dict_node_t* current = table->entries[i];

            do {
                str_entry(tmp_string, current->key, current->item->line);
                fprintf(out, "%s", tmp_string);
                entry_count++;
                current = current->next;
            } while(current != NULL);
        }
    }

    return entry_count;
}

int print_table(comp_dict_t* table){
    return print_file_table(stdout, table);
}

char* str_entry(char* retbuffer, char* key, int line){
    sprintf(retbuffer, "ENTRY: %s;\n\tLine: %d;\n\n", key, line);
    return retbuffer;
}
