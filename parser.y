/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include <stdio.h>
#include "main.h"
%}

/* Declaração dos tokens da linguagem */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%
/* Regras (e ações) da gramática */

programa:
    decl-global programa
    | func programa
    | %empty
;

decl-global:
    decl-local
    | tipo identificador '[' expressao ']' ';'
;

decl-local:
    tipo identificador
;

func:
    cabecalho corpo
;

cabecalho:
    tipo nome-func '(' lista-parametros ')'
    | tipo nome-func '(' ')'
;

corpo:
    bloco-comando
;

bloco-comando:
    '{' comando '}'
;

lista-parametros:
    decl-local
    | decl-local ',' lista-parametros
;

tipo:
    TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL
    | TK_PR_CHAR
    | TK_PR_STRING
;

comando:
    decl-local
    | input
    | output
    | %empty
;

input:
    TK_PR_INPUT identificador
;

output:
    TK_PR_OUTPUT lista-expressao
;

nome-func:
    identificador
;

identificador:
    TK_IDENTIFICADOR
;

expressao:
    identificador
    | expressao operador expressao
    | literal
;

lista-expressao:
    expressao
    | ',' lista-expressao
;

operador:
    '+'
    | '-'
    | '*'
    | '/'
    | '='
    | TK_OC_EQ
    | '<'
    | TK_OC_LE
    | '>'
    | TK_OC_GE
    | TK_OC_NE
    | TK_OC_AND
    | TK_OC_OR
;

literal:
    TK_LIT_CHAR
    | TK_LIT_FALSE
    | TK_LIT_FLOAT
    | TK_LIT_INT
    | TK_LIT_STRING
    | TK_LIT_TRUE
;

%%
