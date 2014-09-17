/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "iks_ast.h"

int parser_return;
int max_id_tabela;

comp_tree_t* arvore_sintatica;
%}

//%define parse.error verbose
//%define parse.trace
%union {
    char* symbol_name;
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
%token<symbol_name> TK_LIT_INT
%token<symbol_name> TK_LIT_FLOAT
%token<symbol_name> TK_LIT_FALSE
%token<symbol_name> TK_LIT_TRUE
%token<symbol_name> TK_LIT_CHAR
%token<symbol_name> TK_LIT_STRING
%token<symbol_name> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type<node> start
%type<node> programa
%type<node> decl-global
%type<node> decl-local
%type<node> decl-parametro
%type<node> func
%type<node> chamada-funcao
%type<node> lista-argumentos
%type<node> corpo
%type<node> bloco-comando
%type<node> sequencia
%type<node> lista-parametros
%type<node> tipo
%type<node> comando
%type<node> condicional
%type<node> laco
%type<node> input
%type<node> output
%type<node> expressao
%type<node> expressao-aritmetica
%type<node> expressao-logica
%type<node> atribuicao
%type<node> lista-expressao
%type<node> literal
%type<node> do
%type<node> while
%type<node> return

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
     }
;

programa:
    decl-global programa {
        $$ = $1;

        /*parser_return = IKS_SYNTAX_SUCESSO;*/
        /*return parser_return;*/
    }
    | func programa {
        connect_nodes((comp_tree_t *)$1, (comp_tree_t *)$2);
        $$ = $1;

        /*parser_return = IKS_SYNTAX_SUCESSO;*/
        /*return parser_return;*/
    }
    | /* %empty */ {
        $$ = NULL;

        /*parser_return = IKS_SYNTAX_SUCESSO;*/
        /*return parser_return;*/
    }
    | error {
        $$ = NULL;
        yyerrok;
        yyclearin;

        /*parser_return = IKS_SYNTAX_ERRO;*/
        /*return parser_return;*/
    }
;

decl-global:
    decl-local ';' {
        $$ = $1;
    }
    | tipo TK_IDENTIFICADOR '[' expressao ']' ';' {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL);
        node_identificador->next_brother = $4;
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador);
    }
    | laco { yyerror("Não são permitidos laços fora do escopo de função"); }
    | condicional { yyerror("Não são permitidas expressões condicionais fora do escopo de função"); }
    | atribuicao { yyerror("Não são permitidas atribuições fora do escopo de função"); }
;

decl-local:
    tipo TK_IDENTIFICADOR {
        $$ = NULL;
    }
;

decl-parametro:
    tipo TK_IDENTIFICADOR {
        $$ = NULL;
    }
;

func:
    tipo TK_IDENTIFICADOR '(' lista-parametros ')' corpo
    {
        // Revisar
        /*$4->next_brother = $6;*/
        $$ = create_node(IKS_AST_FUNCAO, $2, $6);
    }
    | tipo TK_IDENTIFICADOR '(' ')' corpo
    {
        $$ = create_node(IKS_AST_FUNCAO, $2, $5);
    }
;

chamada-funcao:
    TK_IDENTIFICADOR '(' ')'
    {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL);
        $$ = create_node(IKS_AST_CHAMADA_DE_FUNCAO, NULL, node_identificador);
    }
    | TK_IDENTIFICADOR '(' lista-argumentos ')'
    {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL);
        node_identificador->next_brother = $3;
        $$ = create_node(IKS_AST_CHAMADA_DE_FUNCAO, NULL, node_identificador);
    }
;

lista-argumentos:
    expressao
    | expressao ',' lista-argumentos
    {
        connect_nodes($1, $3);
        $$ = $1;
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
        $$ = $1;
    }
    | comando ';' sequencia 
    {
        connect_nodes((comp_tree_t *)$1, (comp_tree_t *)$3);
    }
    | '{' sequencia '}'
    {
        $$ = create_node(IKS_AST_BLOCO, NULL, $2);
    }
    | ';'
    {
        $$ = NULL;
    }
    | '{' '}' {
        $$ = create_node(IKS_AST_BLOCO, NULL, NULL);
    }
    | laco {
      $$ = $1;
    }
    | laco ';' {
      $$ = $1;
    }
    
;

// Revisar
lista-parametros:
    decl-parametro {
        /*$$ = $1;*/
        $$ = NULL;
    }
    | decl-parametro ',' lista-parametros
    {
        /*connect_nodes($1, $3);*/
        /*$$ = $1;*/
        $$ = NULL;
    }
;

tipo:
    TK_PR_INT
    {
        $$ = NULL;
        /*$$ = IKS_SIMBOLO_LITERAL_INT;*/
    }
    | TK_PR_FLOAT
    {
        $$ = NULL;
        /*$$ = IKS_SIMBOLO_LITERAL_FLOAT;*/
    }
    | TK_PR_BOOL
    {
        $$ = NULL;
        /*$$ = IKS_SIMBOLO_LITERAL_BOOL;*/
    }
    | TK_PR_CHAR
    {
        $$ = NULL;
        /*$$ = IKS_SIMBOLO_LITERAL_CHAR;*/
    }
    | TK_PR_STRING
    {
        $$ = NULL;
        /*$$ = IKS_SIMBOLO_LITERAL_STRING;*/
    }
;

comando:
    decl-local {
        $$ = $1;
    }
    | input {
        $$ = $1;
    }
    | output {
        $$ = $1;
    }
    | condicional {
        $$ = $1;
    }
    | chamada-funcao {
        $$ = $1;
    }
   /* | laco {
        $$ = $1;
    }*/
    | return {
        $$ = $1;
    }
    | atribuicao {
        $$ = $1;
    }
    | '{' sequencia '}'
    {
        $$ = create_node(IKS_AST_BLOCO, NULL, $2);
    }
    | '{' '}' {
	fprintf(stderr, "Entrou no bloco vazio");
        $$ = create_node(IKS_AST_BLOCO, NULL, NULL);
    }
    | ';' {
        $$ = NULL;
    }
;

condicional:
    TK_PR_IF '(' expressao ')' TK_PR_THEN comando {
        $3->next_brother = $6;
        $$ = create_node(IKS_AST_IF_ELSE, NULL, $3);
    }
    | TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando {
        $3->next_brother = $6;
        $6->next_brother = $8;
        $$ = create_node(IKS_AST_IF_ELSE, NULL, $3);
    }
    | TK_PR_IF '(' expressao ')' comando TK_PR_ELSE comando {
        $$ = NULL;
        yyerror("É obrigatório utilizar o 'then' ao usar o condicional 'if'");
    }
;

laco:
  while '(' expressao-logica ')' do comando {
        $1->next_brother = $6;
        create_node(IKS_AST_WHILE_DO, NULL, $1);
  }
  | do comando while '(' expressao-logica ')' {

  }
;

input:
    TK_PR_INPUT TK_IDENTIFICADOR {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL);
        $$ = create_node(IKS_AST_INPUT, NULL, node_identificador);
    }
;

output:
    TK_PR_OUTPUT lista-expressao {
        $$ = create_node(IKS_AST_OUTPUT, NULL, $2);
    }
;

expressao:
    TK_IDENTIFICADOR {
        $$ = create_node(IKS_AST_IDENTIFICADOR, $1, NULL);
    }
    | literal {
        $$ = $1;
    }
    | chamada-funcao {
        $$ = $1;
    }
    | TK_IDENTIFICADOR '[' expressao ']' {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL);
        node_identificador->next_brother = $3;
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador);
    }
    | '(' expressao ')' {
        $$ = $2;
    }
    | expressao-aritmetica {
        $$ = $1;
    }
    | expressao-logica {
        $$ = $1;
    }
    | '!' expressao {
        comp_tree_t* unario = create_node(IKS_AST_IDENTIFICADOR, "!", NULL);
        connect_nodes(unario, $2);
        $$ = unario;
        /*$$ = $2;*/
    }
    | '-' expressao %prec UMINUS {
        comp_tree_t* unario = create_node(IKS_AST_IDENTIFICADOR, "-", NULL);
        connect_nodes(unario, $2);
        $$ = unario;
        /*$$ = $2;*/
    }
;


expressao-aritmetica:
    expressao '+' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_SOMA, NULL, $1);
    }
    | expressao '-' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_SUBTRACAO, NULL, $1);
    }
    | expressao '*' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_MULTIPLICACAO, NULL, $1);
    }
    | expressao '/' expressao 
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_DIVISAO, NULL, $1);
    }
;

expressao-logica:
    expressao TK_OC_EQ expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_IGUAL, NULL, $1);
    }
    | expressao '<' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_L, NULL, $1);
    }
    | expressao TK_OC_LE expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_LE, NULL, $1);
    }
    | expressao '>' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_G, NULL, $1);
    }
    | expressao TK_OC_GE expressao 
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_GE, NULL, $1);
    }
    | expressao TK_OC_NE expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_DIF, NULL, $1);
    }
    | expressao TK_OC_AND expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_E, NULL, $1);
    }
    | expressao TK_OC_OR expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_OU, NULL, $1);
    }
;

atribuicao:
    TK_IDENTIFICADOR '=' expressao
    {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL);
        node_identificador->next_brother = $3;

        $$ = create_node(IKS_AST_ATRIBUICAO, NULL, node_identificador);

    }
    | TK_IDENTIFICADOR '[' expressao ']' '=' expressao
    {
        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL);
        comp_tree_t* vetor = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador);
        $$ = create_node(IKS_AST_ATRIBUICAO, NULL, node_identificador);
        $$->next_brother = $6;
    }
;

lista-expressao:
    expressao {
        $$ = $1;
    }
    | expressao ',' lista-expressao
    {
        connect_nodes($1, $3);
        $$ = $1;
    }
;

literal:
    TK_LIT_CHAR {
        $$ = create_node(IKS_AST_LITERAL, $1, NULL);
    }
    | TK_LIT_FALSE {
        $$ = create_node(IKS_AST_LITERAL, $1, NULL);
    }
    | TK_LIT_FLOAT {
        $$ = create_node(IKS_AST_LITERAL, $1, NULL);
    }
    | TK_LIT_INT {
        $$ = create_node(IKS_AST_LITERAL, $1, NULL);
    }
    | TK_LIT_STRING {
        $$ = create_node(IKS_AST_LITERAL, $1, NULL);
    }
    | TK_LIT_TRUE {
        $$ = create_node(IKS_AST_LITERAL, $1, NULL);
    }
;

do:
  TK_PR_DO {
    $$ = NULL;
  }
;

while:
  TK_PR_WHILE {
    $$ = NULL;
  }
;

return:
    TK_PR_RETURN expressao
    {
        $$ = create_node(IKS_AST_RETURN, NULL, $2);
    }
;

%%
