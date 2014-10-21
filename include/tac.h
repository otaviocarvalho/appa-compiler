#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "definitions.h"

typedef struct comp_list_tac_t {
    struct comp_list_tac_t* tac_prev;
    struct comp_list_tac_t* tac_next;
    int tipo;
    char v1[1000], v2[1000], v3[1000];
} comp_list_tac_t;

void print_tac(comp_list_tac_t*);
void print_tac_item(comp_list_tac_t*);
comp_list_tac_t* cria_tac_vazio();
void conecta_tacs_irmaos(comp_list_tac_t*);
comp_list_tac_t* conecta_tacs(comp_list_tac_t*, comp_list_tac_t*);
char* criar_label();
char* criar_registrador();

comp_list_tac_t* montar_tac(int, char*, char*, char*);
