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

%left TK_OC_OR
%left TK_OC_AND
%nonassoc TK_OC_EQ TK_OC_NE
%nonassoc '<' '>' TK_OC_LE TK_OC_GE
%left '+' '-'
%left '*' '/'
%left UMINUS
%right TK_PR_WHILE
%right TK_PR_ELSE TK_PR_THEN

%%
/* Regras (e ações) da gramática */

programa:
    decl-global programa
    | func programa
    | /* %empty */
;

decl-global:
    decl-local ';'
    | tipo identificador '[' expressao ']' ';'
;

decl-local:
    tipo identificador
;

decl-parametro:
    tipo identificador
;

func:
    cabecalho corpo
;

chamada-funcao:
    identificador '(' ')'
    | identificador '(' lista-argumentos ')'
;

lista-argumentos:
    expressao
    | expressao ',' lista-argumentos
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
    decl-parametro
    | decl-parametro ',' lista-parametros
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
    | condicional
    | chamada-funcao
    | laco
    | return
    | atribuicao
    | sequencia
    | '{' comando '}'
    | /* %empty */
;

sequencia:
   comando ';' comando
;

condicional:
    TK_PR_IF '(' expressao-logica ')' TK_PR_THEN comando2
    | TK_PR_IF '(' expressao-logica ')' TK_PR_THEN comando2 TK_PR_ELSE comando2
;

laco:
  while '(' expressao-logica ')' do comando2
  | do comando2 while '(' expressao-logica ')'
;

comando2:
    decl-local
    | input
    | output
    | condicional
    | chamada-funcao
    | laco
    | return
    | atribuicao
    | comando2 ';' comando2
    | '{' comando2 '}'

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
    | literal
    | chamada-funcao
    | identificador '[' expressao ']'
    | '(' expressao ')'
    | expressao-aritmetica
    | expressao-logica
;


expressao-aritmetica:
    expressao '+' expressao
    |expressao '-' expressao
    |expressao '*' expressao
    |expressao '/' expressao
    |'-' expressao %prec UMINUS
;

expressao-logica:
    expressao TK_OC_EQ expressao
    | expressao '<' expressao
    | expressao TK_OC_LE expressao
    | expressao '>' expressao
    | expressao TK_OC_GE expressao
    | expressao TK_OC_NE expressao
    | expressao TK_OC_AND expressao
    | expressao TK_OC_OR expressao
;

atribuicao:
    identificador '=' expressao
    | identificador '[' expressao ']' '=' expressao
;

lista-expressao:
    expressao
    | expressao ',' lista-expressao
;


literal:
    TK_LIT_CHAR
    | TK_LIT_FALSE
    | TK_LIT_FLOAT
    | TK_LIT_INT
    | TK_LIT_STRING
    | TK_LIT_TRUE    
;

do:
  TK_PR_DO
;

while:
  TK_PR_WHILE
;

return:
  TK_PR_RETURN expressao
;

%%
