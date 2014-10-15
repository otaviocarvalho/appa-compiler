/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "iks_ast.h"
#include "comp_tree.h"
#include "definitions.h"

int parser_return;
int cur_dict_id = 1; // Inicializa o id dos dicionários

int cur_line = 1;   // Inicializa o compilador na linha 1
struct comp_dict_t* symbol_table_root; // Ponteiro para a raiz da tabela de símbolos
struct comp_dict_t* symbol_table_cur; // Ponteiro para a tabela de símbolos do escopo corrente

comp_tree_t* arvore_sintatica;
struct comp_stack_dict_t *stack_scope;

comp_dict_item_t* hash_item;
%}

%define parse.error verbose
%define parse.trace

%union {
    char* symbol_name;
    int symbol_val;
    struct comp_dict_item_t* symbol;
    struct comp_tree_t* node;
    struct comp_list_t* symbol_list;
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
%type<symbol_list> decl-parametro
%type<node> func
%type<node> chamada-funcao
%type<symbol_list> lista-argumentos
%type<node> corpo
%type<node> bloco-comando
%type<node> sequencia
%type<symbol_list> lista-parametros
%type<symbol_val> tipo
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
        fprintf(stdout, "programa\n");
        arvore_sintatica = create_node(IKS_AST_PROGRAMA, NULL, $1, NULL);
        $$ = arvore_sintatica;
        /*print_syntax_tree($$);*/
     }
;

programa:
    decl-global programa {
        $$ = $2;

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
    | tipo TK_IDENTIFICADOR '[' expressao ']' ';' {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1,DECLARACAO_VETOR_INDEXADO);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL, hash_item);
        node_identificador->next_brother = $4;
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);
    }
    | laco { yyerror("Não são permitidos laços fora do escopo de função"); }
    | condicional { yyerror("Não são permitidas expressões condicionais fora do escopo de função"); }
    | atribuicao { yyerror("Não são permitidas atribuições fora do escopo de função"); }
;

decl-local:
    tipo TK_IDENTIFICADOR {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VARIAVEL);

        $$ = NULL;
    }
;

decl-parametro:
    tipo TK_IDENTIFICADOR {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VARIAVEL);

        $$ = list_create(hash_item);
    }
;

func:
    tipo TK_IDENTIFICADOR '(' lista-parametros ')' { create_table(cur_dict_id++); } corpo { destroy_table(cur_dict_id--); }
    {
        fprintf(stdout, "declaracao funcao corpo\n");
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_FUNCAO);

        $$ = create_node(IKS_AST_FUNCAO, $2, $7, hash_item);

        list_func_connect($$, $4, hash_item);
        verifica_return($$, $2, $1);
    }
    | tipo TK_IDENTIFICADOR '(' ')' { create_table(cur_dict_id++); } corpo { destroy_table(cur_dict_id--); }
    {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_FUNCAO);

        $$ = create_node(IKS_AST_FUNCAO, $2, $6, hash_item);

        hash_item->count_args = 0;
        verifica_return($$, $2, $1);

        print_stack_dict(stack_scope);
        print_syntax_tree($6);
        fprintf(stdout, "declaracao funcao\n");
    }
;

chamada-funcao:
    TK_IDENTIFICADOR '(' ')'
    {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_FUNCAO);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        $$ = create_node(IKS_AST_CHAMADA_DE_FUNCAO, $1, node_identificador, hash_item);

        hash_item->count_args = 0;
        verifica_argumentos($$, $1, 0);

        fprintf(stdout, "chamada funcao\n");
    }
    | TK_IDENTIFICADOR '(' lista-argumentos ')'
    {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_FUNCAO);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        $$ = create_node(IKS_AST_CHAMADA_DE_FUNCAO, NULL, node_identificador, NULL);

        list_func_connect($$, $3, hash_item);
        verifica_argumentos($$, $1, $3);
    }
;

lista-argumentos:
    expressao {
        $$ = list_create($1->hash);
    }
    | expressao ',' lista-argumentos
    {
        $$ = list_concat(list_create($1->hash), $3);
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
    comando
    {
        $$ = $1;
    }
    | comando ';' sequencia 
    {
        connect_nodes((comp_tree_t *)$1, (comp_tree_t *)$3);
        $$ = $1;
    }
    |comando ';'
    {
        $$ = $1;
    }
    |decl-local {
        $$ = $1;
    }
    |decl-local ';'{
        $$ = $1;
    }
    | decl-local ';' sequencia 
    {
        $$ = $3;
    }
;

lista-parametros:
    decl-parametro {
        /*$$ = list_create($1);*/
        $$ = $1;
    }
    | decl-parametro ',' lista-parametros
    {
        $$ = list_concat($1, $3);
        /*$$ = NULL;*/
    }
;

tipo:
    TK_PR_INT
    {
        $$ = IKS_INT;
    }
    | TK_PR_FLOAT
    {
        $$ = IKS_FLOAT;
    }
    | TK_PR_BOOL
    {
        $$ = IKS_BOOL;
    }
    | TK_PR_CHAR
    {
        $$ = IKS_CHAR;
    }
    | TK_PR_STRING
    {
        $$ = IKS_STRING;
    }
;

comando:
    input {
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
    | laco {
        $$ = $1;
    }
    | return {
        $$ = $1;
    }
    | atribuicao {
        $$ = $1;
    }
    | '{' { create_table(cur_dict_id++); } sequencia '}' { destroy_table(cur_dict_id--); }
    {
        $$ = create_node(IKS_AST_BLOCO, NULL, $3, NULL);
    }
    | '{' { create_table(cur_dict_id++); } '}' { destroy_table(cur_dict_id--); } {
        $$ = create_node(IKS_AST_BLOCO, NULL, NULL, NULL);
    }
    | ';' {
        $$ = NULL;
    }
    | func{
        $$ = $1;
    }
;

condicional:
    TK_PR_IF '(' expressao ')' TK_PR_THEN comando {
        $3->next_brother = $6;
        $$ = create_node(IKS_AST_IF_ELSE, NULL, $3, NULL);
    }
    | TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando {
        $3->next_brother = $6;
        $6->next_brother = $8;
        $$ = create_node(IKS_AST_IF_ELSE, NULL, $3, NULL);
    }
    | TK_PR_IF '(' expressao ')' comando TK_PR_ELSE comando {
        $$ = NULL;
        yyerror("É obrigatório utilizar o 'then' ao usar o condicional 'if'");
    }
;

laco:
  while '(' expressao ')' do comando {
        $3->next_brother = $6;
        $$ = create_node(IKS_AST_WHILE_DO, NULL, $3, NULL);
  }
  | do comando while '(' expressao ')' {
        $2->next_brother = $5;
        $$ = create_node(IKS_AST_DO_WHILE, NULL, $2, NULL);

  }
;

input:
    TK_PR_INPUT lista-expressao {
        /*hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VARIAVEL);*/

        /*comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL, hash_item);*/
        /*comp_tree_t* nodo_input = create_node(IKS_AST_INPUT, NULL, node_identificador, NULL);*/
        /*$$ = nodo_input;*/

        $$ = create_node(IKS_AST_INPUT, NULL, $2, NULL);

        verifica_input($$);
    }
    /*|  TK_PR_INPUT TK_IDENTIFICADOR '[' expressao ']' {*/
        /*hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VETOR_INDEXADO);*/

        /*comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL, hash_item);*/
        /*comp_tree_t* nodo_input = create_node(IKS_AST_INPUT, NULL, node_identificador, NULL);*/
        /*$$ = nodo_input;*/

        /*verifica_input($$);*/
    /*}*/
;

output:
    TK_PR_OUTPUT lista-expressao {
        $$ = create_node(IKS_AST_OUTPUT, NULL, $2, NULL);

        verifica_output($$);
    }
;

expressao:
    TK_IDENTIFICADOR {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VARIAVEL);
        $$ = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
    }
    | literal {
        $$ = $1;
    }
    | chamada-funcao {
        $$ = $1;
    }
    | TK_IDENTIFICADOR '[' expressao ']' {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, IKS_AST_VETOR_INDEXADO);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        node_identificador->next_brother = $3;
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);
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
        comp_tree_t* unario = create_node(IKS_AST_IDENTIFICADOR, "!", NULL, NULL);
        connect_nodes(unario, $2);
        $$ = unario;
        /*$$ = $2;*/
    }
    | '-' expressao %prec UMINUS {
        comp_tree_t* unario = create_node(IKS_AST_IDENTIFICADOR, "-", NULL, NULL);
        connect_nodes(unario, $2);
        $$ = unario;
        /*$$ = $2;*/
    }
;


expressao-aritmetica:
    expressao '+' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_SOMA, NULL, $1, NULL);
    }
    | expressao '-' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_SUBTRACAO, NULL, $1, NULL);
    }
    | expressao '*' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_MULTIPLICACAO, NULL, $1, NULL);
    }
    | expressao '/' expressao 
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_DIVISAO, NULL, $1, NULL);
    }
;

expressao-logica:
    expressao TK_OC_EQ expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_IGUAL, NULL, $1, NULL);
    }
    | expressao '<' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_L, NULL, $1, NULL);
    }
    | expressao TK_OC_LE expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_LE, NULL, $1, NULL);
    }
    | expressao '>' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_G, NULL, $1, NULL);
    }
    | expressao TK_OC_GE expressao 
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_GE, NULL, $1, NULL);
    }
    | expressao TK_OC_NE expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_DIF, NULL, $1, NULL);
    }
    | expressao TK_OC_AND expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_E, NULL, $1, NULL);
    }
    | expressao TK_OC_OR expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_OU, NULL, $1, NULL);
    }
;

atribuicao:
    TK_IDENTIFICADOR '=' expressao
    {
        fprintf(stdout, "entrou atribuicao\n");
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VARIAVEL);

        int tipo = encontra_tipo($1,DECLARACAO_VARIAVEL);
        if(tipo == IKS_TYPE_NOT_DEFINED){
            fprintf(stdout,"Operador não definido\n\n");
            exit(IKS_TYPE_NOT_DEFINED);
        }

        verifica_atribuicao($3,tipo);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        node_identificador->next_brother = $3;

        $$ = create_node(IKS_AST_ATRIBUICAO, NULL, node_identificador, NULL);
    }
    | TK_IDENTIFICADOR '[' expressao ']' '=' expressao
    {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VETOR_INDEXADO);

        int tipo = encontra_tipo($1,DECLARACAO_VETOR_INDEXADO);
        if(tipo == IKS_TYPE_NOT_DEFINED){
            fprintf(stdout,"Operador não definido\n\n");
            exit(IKS_TYPE_NOT_DEFINED);
        }
        verifica_tipo_indexador($3);
        verifica_atribuicao($6,tipo);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        node_identificador->next_brother = $3;

        comp_tree_t* vetor = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);
        vetor->next_brother = $6;

        $$ = create_node(IKS_AST_ATRIBUICAO, NULL, vetor, NULL);
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
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_CHAR, IKS_CHAR, USO_LITERAL);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
    }
    | TK_LIT_FALSE {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_BOOL, IKS_BOOL, USO_LITERAL);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
    }
    | TK_LIT_FLOAT {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_FLOAT, IKS_FLOAT, USO_LITERAL);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
    }
    | TK_LIT_INT {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_INT, IKS_INT, USO_LITERAL);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
    }
    | TK_LIT_STRING {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_STRING, IKS_STRING, USO_LITERAL);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
    }
    | TK_LIT_TRUE {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_BOOL, IKS_BOOL, USO_LITERAL);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
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
    TK_PR_RETURN expressao {
        hash_item = add_symbol(symbol_table_cur, "return", cur_line, TK_PR_RETURN, IKS_TYPE_NOT_DEFINED, USO_LITERAL);

        $$ = create_node(IKS_AST_RETURN, NULL, $2, hash_item);
    }
;

%%
