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
            free(current->item->value);
            free(current->item);
            free(current);
            while (next != NULL){
                current = next;
                next = current->next;

                free(current->key);
                free(current->item->value);
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
comp_dict_item_t* add_symbol(comp_dict_t* cur_table, char* key, int line, int type){
    // Aloca estruturas de dados para o novo nodo
    comp_dict_node_t* node = malloc(sizeof(comp_dict_node_t));
    node->item = malloc(sizeof(comp_dict_item_t));

    node->key = malloc(sizeof(char)*SIZE_TABLE_KEY);
    strcpy(node->key, key);
    
    node->next = NULL;
    node->item->line = line;
    node->item->type = convert_type_symbol(type);
    node->item->value = alloc_value_symbol(node->item->type, key); // Aloca valor do token de acordo com o tipo
   
    char buffer[SIZE_TABLE_KEY];
    snprintf(buffer,SIZE_TABLE_KEY,"%d",node->item->type);
    strcat(node->key, buffer);
 
    
    // Calcula o hash
    int hash = hash_function(node->key);
    // Inicializa bucket com esse elemento se não encontrar nenhum
    if (cur_table->entries[hash] == NULL) {
        cur_table->entries[hash] = node;
    }
    // Atualiza linha se já existe
    else if ( strcmp(key, cur_table->entries[hash]->key) == 0 ){
        cur_table->entries[hash]->item->line = line;
        free(node->key);
        free(node->item->value);
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

int convert_type_symbol(int type){
    int type_return = 0;
    switch (type)
    {
        case TK_PR_INT:
        {
            type_return = IKS_SIMBOLO_LITERAL_INT;
            break;
        }
        case TK_PR_FLOAT:
        {
            type_return = IKS_SIMBOLO_LITERAL_FLOAT;
            break;
        }
        case TK_PR_CHAR:
        {
            type_return = IKS_SIMBOLO_LITERAL_CHAR;
            break;
        }
        case TK_PR_STRING:
        {
            type_return = IKS_SIMBOLO_LITERAL_STRING;
            break;
        }
        case TK_IDENTIFICADOR:
        {
            type_return = IKS_SIMBOLO_IDENTIFICADOR;
            break;
        }
        case TK_PR_BOOL:
        {
            type_return = IKS_SIMBOLO_LITERAL_BOOL;
            break;
        }
    }

    return type_return;
}

// Define o tipo do simbolo a ser incluído na tabela
void* alloc_value_symbol(int type, char* key){
    switch (type)
    {
        case IKS_SIMBOLO_LITERAL_INT:
        {
            int* value = (int*) malloc(sizeof(int));
            *value = atoi(key);
            return value;
            break;
        }
        case IKS_SIMBOLO_LITERAL_FLOAT:
        {
            float* value = (float*) malloc(sizeof(float));
            *value = atof(key);
            return value;
            break;
        }
        case IKS_SIMBOLO_LITERAL_CHAR:
        {
            char* value = (char*) malloc(sizeof(char));
            // Remoção das aspas simples
            memcpy(value, key+1, strlen(key)-2);
            return value;
            break;
        }
        case IKS_SIMBOLO_LITERAL_BOOL:
        case IKS_SIMBOLO_IDENTIFICADOR:
        {
            char* value = strdup(key);
            return value;
            break;
        }
        case IKS_SIMBOLO_LITERAL_STRING:
        {
            char* value = (char*) malloc(sizeof(char)*strlen(key));
            // Remoção das aspas duplas
            memcpy(value, key+1, strlen(key)-2);
            return value;
            break;
        }
    }

    return NULL;
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
                str_entry(tmp_string, current->key, current->item->line, current->item->type, (void*)current->item->value);
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

char* str_entry(char* retbuffer, char* key, int line, int type, void* value){
    sprintf(retbuffer, "ENTRY: %s;\n\tLine: %d;\n\tType: %d;\n\tValue: %s;\n\n", key, line, type, (char*)value);
    return retbuffer;
}
