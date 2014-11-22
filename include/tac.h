#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "definitions.h"
#include "comp_tree.h"

#define TK_PR_INT 258
#define TK_PR_FLOAT 259
#define TK_PR_BOOL 260
#define TK_PR_CHAR 261
#define TK_PR_STRING 262
#define TK_PR_IF 263
#define TK_PR_THEN 264
#define TK_PR_ELSE 265
#define TK_PR_WHILE 266
#define TK_PR_DO 267
#define TK_PR_INPUT 268
#define TK_PR_OUTPUT 269
#define TK_PR_RETURN 270
#define TK_OC_LE 271
#define TK_OC_GE 272
#define TK_OC_EQ 273
#define TK_OC_NE 274
#define TK_OC_AND 275
#define TK_OC_OR 276
#define TK_LIT_INT 277
#define TK_LIT_FLOAT 278
#define TK_LIT_FALSE 279
#define TK_LIT_TRUE 280
#define TK_LIT_CHAR 281
#define TK_LIT_STRING 282
#define TK_IDENTIFICADOR 283
#define TOKEN_ERRO 284
#define UMINUS 285

typedef struct comp_list_tac_t {
    struct comp_list_tac_t* tac_prev;
    struct comp_list_tac_t* tac_next;
    int tipo;
    char v1[1000], v2[1000], v3[1000];
} comp_list_tac_t;

typedef struct comp_list_tac_vector_t {
    int dimensao;
    struct comp_list_tac_t* tac;
    struct comp_list_tac_vector_t* next;
} comp_list_tac_vector_t;

void print_tac(comp_list_tac_t*);
void print_tac_item(comp_list_tac_t*, FILE*);
void conecta_tacs_irmaos(comp_list_tac_t*);
comp_list_tac_t* conecta_tacs(comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t* cria_copia_conecta_tacs(comp_list_tac_vector_t*);
void conecta_bloco_ultimo_com_proximo(comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t* busca_bloco_ultimo(comp_list_tac_t*);
char* criar_label();
char* criar_registrador();

comp_list_tac_t* criar_tac();
comp_list_tac_t* montar_tac(int, char*, char*, char*);
comp_list_tac_t *criar_tac_expressao(int, comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t *criar_tac_literal(int, int, int, char*, int, int, comp_list_tac_vector_t*, comp_list_tac_vector_t*);
comp_list_tac_t* criar_tac_funcao(char*, comp_list_tac_t*);
comp_list_tac_t* criar_tac_chamada_funcao(char*, comp_list_tac_t*);
comp_list_tac_t* criar_tac_atribuicao(char*, comp_list_tac_t*, int, int);
comp_list_tac_t* criar_tac_atribuicao_vetor(char*, comp_list_tac_t*, comp_list_tac_vector_t*, comp_list_tac_vector_t*, int, int, int);
comp_list_tac_t *cria_tac_if(comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t *cria_tac_if_else(comp_list_tac_t*, comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t *cria_tac_do_while(comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t *cria_tac_while_do(comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t* calcula_tac_lista_param_desloc(comp_list_tac_vector_t*, comp_list_tac_vector_t*, char*, int, int);
comp_list_tac_t* criar_tac_expressao_logica(int , comp_list_tac_t* , comp_list_tac_t*);
comp_list_tac_t* criar_tac_jump_main();

comp_list_tac_vector_t* list_tac_concat(comp_list_tac_vector_t* list_a, comp_list_tac_vector_t* list_b);
comp_list_tac_vector_t* list_tac_create_item(comp_list_tac_t*);
int list_tac_count(comp_list_tac_vector_t*);

int calcula_dimensao_arranjo(comp_list_tac_vector_t*);

void otimiza_os_barato(comp_list_tac_t*);