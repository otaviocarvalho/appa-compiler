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
};

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
%token<symbol> TK_LIT_INT
%token<symbol> TK_LIT_FLOAT
%token<symbol> TK_LIT_FALSE
%token<symbol> TK_LIT_TRUE
%token<symbol> TK_LIT_CHAR
%token<symbol> TK_LIT_STRING
%token<symbol> TK_IDENTIFICADOR
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

programa:{
	arvore_tabela = inicializa_arvore();
	tabela_atual = arvore_tabela;
	max_id_tabela = 1;
}
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
    | tipo identificador '[' expressao ']' ';'{
	$1->next_brother = $2;
	$2->children = $4;
}
    | laco { yyerror("Não são permitidos laços fora do escopo de função"); }
    | condicional { yyerror("Não são permitidas expressões condicionais fora do escopo de função"); }
    | atribuicao { yyerror("Não são permitidas atribuições fora do escopo de função"); }
;

decl-local:
    tipo identificador
    {
	$1->next_brother = $2;
	$$ = $1;
    }
;

decl-parametro:
    tipo identificador
    {
	$1->next_brother = $2;
	$$ = $1;
    }
;

func:
    cabecalho corpo
    {
      $1->next_brother = $2;
      $$ = $1;
    }
;

chamada-funcao:
    identificador '(' ')'
    {
      $$ = $1;
    }
    | identificador '(' lista-argumentos ')'
    {
      $1->next_brother = $3;
      $$ = $1;
    }
;

lista-argumentos:
    expressao
    | expressao ',' lista-argumentos
    {
      $1->next_brother=$3
    }
;

cabecalho:
    tipo nome-func '(' lista-parametros ')'
    {
      $1->next_brother=$4
    }
    | tipo nome-func '(' ')'
;

corpo:
    bloco-comando
;

bloco-comando:
    '{' sequencia '}'
    {
      $$ = $2;
    }
    | '{' '}'
;

sequencia:
    comando ';'
    { 
      $1->next_brother = $2; 
      $$ = $1;
    }
    | comando ';' sequencia 
    { 
      $1->next_brother = $3;
      $$ = $1;
    }
    | '{' sequencia '}'
    { 
      $$ = $2
    }
    | ';' 
    { 
      $$ = $1;
    }
;

lista-parametros:
    decl-parametro 
    | decl-parametro ',' lista-parametros 
    { 
      $1->next_brother = $3; 
      $$ = $1;
    }
;

tipo:
    TK_PR_INT 
    { 
      $$ = $1;
    }
    | TK_PR_FLOAT 
    { 
      $$ = $1;
    }
    | TK_PR_BOOL 
    { 
      $$ = $1;
    }
    | TK_PR_CHAR 
    { 
      $$ = $1;
    }
    | TK_PR_STRING 
    { 
      $$ = $1;
    }
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
    { 
      $$->next_brother = $2; 
    }
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
    TK_PR_INPUT identificador{
     $1->next_brother = $2;
     $$ = $1;
    }
;

output:
    TK_PR_OUTPUT lista-expressao{
     $1->next_brother = $2;
     $$ = $1;
    }
;

nome-func:
    identificador
;

identificador:
    TK_IDENTIFICADOR { $$ = $1; }
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
    { $1->next_brother = $3;
      $$ = $1;
    }
    | expressao '-' expressao 
    { $1->next_brother = $3; 
      $$ = $1; 
    }
    | expressao '*' expressao 
    { $1->next_brother = $3;
      $$ = $1; 
    }
    | expressao '/' expressao 
    { $1->next_brother = $3;
      $$ = $1; 
    }
;

expressao-logica:
    expressao TK_OC_EQ expressao 
    { 
      $1->next_brother = $3; 
      $$ = $1; 
    }
    | expressao '<' expressao 
    { 
      $1->next_brother = $3;
      $$ = $1;
    }
    | expressao TK_OC_LE expressao 
    { 
      $1->next_brother = $3;
      $$ = $1;
    }
    | expressao '>' expressao 
    {
      $1->next_brother = $3;
      $$ = $1; 
    }
    | expressao TK_OC_GE expressao 
    { 
      $1->next_brother = $3;
      $$ = $1; 
    }
    | expressao TK_OC_NE expressao 
    { 
      $1->next_brother = $3;
      $$ = $1; 
    }
    | expressao TK_OC_AND expressao 
    { 
      $1->next_brother = $3;
      $$ = $1;
    }
    | expressao TK_OC_OR expressao 
    { 
      $1->next_brother = $3;
      $$ = $1; 
    }
;

atribuicao:
    identificador '=' expressao 
    { 
      $1->next_brother = $3; 
      $$ = $1;
    }
    | identificador '[' expressao ']' '=' expressao 
    { 
      $1->next_brother = $3;
      $3->next_brother = $6;
    }
;

lista-expressao:
    expressao
    | expressao ',' lista-expressao
    {
      $1->next_brother = $3;
      $$ = $1;
    }
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
  {
    $1->next_brother = $2;
    $$ = $1;
  }
;

%%
