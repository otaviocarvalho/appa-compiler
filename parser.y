/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include <stdio.h>
#include "main.h"
#include "iks_ast.h"

int parser_return;
int max_id_tabela;

comp_tree_t* arvore_sintatica;
%}

%define parse.error verbose
%define parse.trace
%union {
    struct comp_dict_item_t* symbol;
    struct comp_tree_t *node;
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

%type<node> start
%type<node> programa
%type<node> decl-global
%type<node> decl-local
/*%type<node> decl-parametro*/
%type<node> func
/*%type<node> chamada-funcao*/
/*%type<node> lista-argumentos*/
%type<node> cabecalho
%type<node> corpo
%type<node> bloco-comando
%type<node> sequencia
/*%type<node> lista-parametros*/
%type<node> tipo
/*%type<node> comando*/
/*%type<node> condicional*/
/*%type<node> laco*/
/*%type<node> input*/
/*%type<node> output*/
/*%type<node> nome-func*/
%type<node> identificador
/*%type<node> expressao*/
/*%type<node> expressao-aritmetica*/
/*%type<node> expressao-logica*/
/*%type<node> atribuicao*/
/*%type<node> lista-expressao*/
/*%type<node> literal*/
/*%type<node> do*/
/*%type<node> while*/
/*%type<node> return*/

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
start:
     programa {
        arvore_sintatica = create_node(IKS_AST_PROGRAMA, NULL, $1);
        $$ = arvore_sintatica;
        /*$$->next_brother = $1;*/
        /*arvore_sintatica = $1;*/
     }
;

programa:
    decl-global programa {
        /*arvore_sintatica = create_node(IKS_AST_PROGRAMA, "programa", NULL);*/
        /*arvore_sintatica->next_brother = $1;*/
        /*$1->next_brother = $2;*/

        /*$1->next_brother = $2;*/
        $$ = $1;
        /*parser_return = IKS_SYNTAX_SUCESSO;*/
        /*return parser_return;*/
    }
    | func programa {
	$1->next_brother = $2;
        $$ = $1;        
        /*$$ = $1;*/
        /*parser_return = IKS_SYNTAX_SUCESSO;*/
        /*return parser_return;*/
    }
    | /* %empty */ {
        $$ = NULL;
        /*parser_return = IKS_SYNTAX_SUCESSO;*/
        /*return parser_return;*/
    }
    | error {
        /*$$ = NULL;*/
        /*yyerrok;*/
        /*yyclearin;*/
        /*parser_return = IKS_SYNTAX_ERRO;*/
        /*return parser_return;*/
    }
;

decl-global:
    decl-local ';' {
        $$ = $1;
    }
    | tipo identificador '[' expressao ']' ';' {
        /*$$ = create_node(IKS_AST_IDENTIFICADOR, "IKS_AST_IDENTIFICADOR", NULL);*/
        /*$$->next_brother = $4;*/
    }
    | laco { yyerror("Não são permitidos laços fora do escopo de função"); }
    | condicional { yyerror("Não são permitidas expressões condicionais fora do escopo de função"); }
    | atribuicao { yyerror("Não são permitidas atribuições fora do escopo de função"); }
;

decl-local:
    tipo TK_IDENTIFICADOR {
        /*$$ = create_node(IKS_AST_IDENTIFICADOR, "identificador", NULL);*/
        $$ = NULL;
    }
;

decl-parametro:
    tipo identificador {
        /*$1->next_brother = $2;*/
        /*$$ = $1;*/
    }
;

func:
    cabecalho corpo
    {
	
	$2 = create_node(IKS_AST_FUNCAO,"corpo função", $2);
        $$ = $1;
      
      /*$$ = $1;*/
    }
;

chamada-funcao:
    identificador '(' ')'
    {
        /*$$ = $1;*/
    }
    | identificador '(' lista-argumentos ')'
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
    }
;

lista-argumentos:
    expressao
    | expressao ',' lista-argumentos
    {
        /*$1->next_brother=$3*/
    }
;

cabecalho:
    /*tipo nome-func '(' lista-parametros ')'*/
    tipo identificador '(' lista-parametros ')'
    {
        $$ = $2;
        /*$1->next_brother=$4*/
    }
    /*| tipo nome-func '(' ')'*/
    | tipo identificador '(' ')' {
        $$ = $2;
    }
;

corpo:
    bloco-comando
    {
      $$ = $1;
    }
;

bloco-comando:
    '{' sequencia '}'
    {
        $$ = $2;
    }
    |
    '{' '}'
    {
      $$ = NULL;
    }
;

sequencia:
    comando ';'
    {
        /*$1->next_brother = $2; */
        /*$$ = $1;*/
    }
    | comando ';' sequencia 
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
    }
    | '{' sequencia '}'
    {
        /*$$ = $2*/
    }
    | ';'
    {
        $$ = create_node(IKS_AST_BLOCO, "ponto e vírguala", NULL);
    }
;

lista-parametros:
    decl-parametro 
    | decl-parametro ',' lista-parametros
    {
        /*$1->next_brother = $3; */
        /*$$ = $1;*/
    }
;

tipo:
    TK_PR_INT
    {
        /*$$ = IKS_SIMBOLO_LITERAL_INT;*/
    }
    | TK_PR_FLOAT
    {
        /*$$ = IKS_SIMBOLO_LITERAL_FLOAT;*/
    }
    | TK_PR_BOOL
    {
        /*$$ = IKS_SIMBOLO_LITERAL_BOOL;*/
    }
    | TK_PR_CHAR
    {
        /*$$ = IKS_SIMBOLO_LITERAL_CHAR;*/
    }
    | TK_PR_STRING
    {
        /*$$ = IKS_SIMBOLO_LITERAL_STRING;*/
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
        /*$$->next_brother = $2; */
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
        /*$1->next_brother = $2;*/
        /*$$ = $1;*/
    }
;

output:
    TK_PR_OUTPUT lista-expressao{
        /*$1->next_brother = $2;*/
        /*$$ = $1;*/
    }
;

/*nome-func:*/
    /*identificador {*/
        /*$$ = $1;*/
    /*}*/
/*;*/

identificador:
    TK_IDENTIFICADOR {
        $$ = create_node(IKS_AST_IDENTIFICADOR, "identificador função", NULL);
        /*$$ = IKS_SIMBOLO_IDENTIFICADOR;*/
    }
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
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
    }
    | expressao '-' expressao
    {
        /*$1->next_brother = $3; */
        /*$$ = $1; */
    }
    | expressao '*' expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1; */
    }
    | expressao '/' expressao 
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1; */
    }
;

expressao-logica:
    expressao TK_OC_EQ expressao
    {
        /*$1->next_brother = $3; */
        /*$$ = $1; */
    }
    | expressao '<' expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
    }
    | expressao TK_OC_LE expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
    }
    | expressao '>' expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1; */
    }
    | expressao TK_OC_GE expressao 
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1; */
    }
    | expressao TK_OC_NE expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1; */
    }
    | expressao TK_OC_AND expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
    }
    | expressao TK_OC_OR expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1; */
    }
;

atribuicao:
    identificador '=' expressao
    {
        /*$1->next_brother = $3; */
        /*$$ = $1;*/
    }
    | identificador '[' expressao ']' '=' expressao
    {
        /*$1->next_brother = $3;*/
        /*$3->next_brother = $6;*/
    }
;

lista-expressao:
    expressao
    | expressao ',' lista-expressao
    {
        /*$1->next_brother = $3;*/
        /*$$ = $1;*/
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
        /*$1->next_brother = $2;*/
        /*$$ = $1;*/
  }
;

%%
