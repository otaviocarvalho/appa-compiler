#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "definitions.h"

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

void print_tac(comp_list_tac_t*);
void print_tac_item(comp_list_tac_t*);
void conecta_tacs_irmaos(comp_list_tac_t*);
comp_list_tac_t* conecta_tacs(comp_list_tac_t*, comp_list_tac_t*);
char* criar_label();
char* criar_registrador();

comp_list_tac_t* criar_tac();
comp_list_tac_t* montar_tac(int, char*, char*, char*);
comp_list_tac_t *criar_tac_expressao(int, comp_list_tac_t*, comp_list_tac_t*);
comp_list_tac_t *criar_tac_literal(int, char*);
comp_list_tac_t* criar_tac_funcao(char*, comp_list_tac_t*);
comp_list_tac_t* criar_tac_chamada_funcao(char*, comp_list_tac_t*);
comp_list_tac_t* criar_tac_atribuicao(char*, comp_list_tac_t*, int);
comp_list_tac_t *cria_tac_if(comp_list_tac_t*, comp_list_tac_t*);

