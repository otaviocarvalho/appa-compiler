/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include <stdio.h>
#include "main.h"

int parser_return;
%}

%define parse.error verbose
%define parse.trace
%union {
    struct comp_dict_item_t* symbol;
    int var_type;
};

/* Declaração dos tokens da linguagem */
%token<var_type> TK_PR_INT
%token<var_type> TK_PR_FLOAT
%token<var_type> TK_PR_BOOL
%token<var_type> TK_PR_CHAR
%token<var_type> TK_PR_STRING
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
%left '!'
%left '+' '-'
%left '*' '/'
%left '(' ')'
%left UMINUS
%right TK_PR_ELSE TK_PR_THEN

%%
/* Regras (e ações) da gramática */

programa:
    decl-global programa {
        parser_return = IKS_SYNTAX_SUCESSO;
        return parser_return;
    }
    | func programa {
        parser_return = IKS_SYNTAX_SUCESSO;
        return parser_return;
    }
    | /* %empty */ {
        parser_return = IKS_SYNTAX_SUCESSO;
        return parser_return;
    }
    | error {
        yyerrok;
        yyclearin;
        parser_return = IKS_SYNTAX_ERRO;
        return parser_return;
    }
;

decl-global:
    decl-local ';'
    | tipo identificador '[' expressao ']' ';'
    | laco { yyerror("Não são permitidos laços fora do escopo de função"); }
    | condicional { yyerror("Não são permitidas expressões condicionais fora do escopo de função"); }
    | atribuicao { yyerror("Não são permitidas atribuições fora do escopo de função"); }
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
    '{' sequencia '}'
    | '{' '}'
;

sequencia:
    comando ';'
    | comando ';' sequencia
    | '{' sequencia '}'
    | ';'
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
    | '{' sequencia '}'
    | ';'
;

condicional:
    TK_PR_IF '(' expressao-logica ')' TK_PR_THEN comando
    | TK_PR_IF '(' expressao-logica ')' TK_PR_THEN comando TK_PR_ELSE comando
    | TK_PR_IF '(' expressao-logica ')' comando TK_PR_ELSE comando {
        yyerror("É obrigatório utilizar o 'then' ao usar o condicional 'if'");
    }
;

laco:
  while '(' expressao-logica ')' do comando
  | do comando while '(' expressao-logica ')'
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
    | literal
    | chamada-funcao
    | identificador '[' expressao ']'
    | '(' expressao ')'
    | expressao-aritmetica
    | expressao-logica
    | '!' expressao
    | '-' expressao %prec UMINUS
;


expressao-aritmetica:
    expressao '+' expressao
    | expressao '-' expressao
    | expressao '*' expressao
    | expressao '/' expressao
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
