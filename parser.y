/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "tac.h"
#include "iks_ast.h"
#include "comp_tree.h"
#include "definitions.h"

int parser_return;
int cur_dict_id = 1; // Inicializa o id dos dicionários
int deslocamento_global = 0; //Deslocamento global em relação ao rbss

int cur_line = 1;   // Inicializa o compilador na linha 1
struct comp_dict_t* symbol_table_root; // Ponteiro para a raiz da tabela de símbolos
struct comp_dict_t* symbol_table_cur; // Ponteiro para a tabela de símbolos do escopo corrente

comp_tree_t* arvore_sintatica;
struct comp_stack_dict_t *stack_scope;

comp_dict_item_t* hash_item;
comp_dict_item_t* hash_item_func = NULL;

comp_list_tac_t* tac_registro_ativacao;
comp_list_tac_t* tac_fim_registro_ativacao;

int linha_escopo_estatico = -1;
int linha_escopo_dinamico = -1;
%}

%define parse.error verbose
%define parse.trace

%union {
    char* symbol_name;
    int symbol_val;
    struct comp_dict_item_t* symbol;
    struct comp_tree_t* node;
    struct comp_list_t* symbol_list;
    struct comp_list_tac_vector_t* symbol_tac_list;
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
%type<node> func
%type<node> chamada-funcao
%type<node> corpo
%type<node> bloco-comando
%type<node> sequencia
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
%type<symbol_list> decl-parametro
%type<symbol_list> lista-argumentos
%type<symbol_tac_list> lista-expressao-vetor
%type<symbol_list> lista-parametros
%type<symbol_tac_list> lista-expressao-dimensoes-vetor
%type<node> literal
%type<node> do
%type<node> while
%type<node> return
%type<node> expressao-parametro-chamada-funcao

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
        // Cria TAC para zerar o rsp
        comp_list_tac_t* tac_programa = conecta_tacs(criar_tac_zera_sp(), criar_tac_jump_main());

        // Cria TAC para executar a função main
        tac_programa = conecta_tacs(tac_programa, $1->tac);

        // Cria TAC para o fim do código
        tac_programa = conecta_tacs(tac_programa, criar_tac_fim_programa());

        arvore_sintatica = create_node(IKS_AST_PROGRAMA, NULL, $1, NULL);
        $$ = arvore_sintatica;

        print_tac(tac_programa);
        /*arvore_sintatica = create_node(IKS_AST_PROGRAMA, NULL, $1, NULL);*/
        /*$$ = arvore_sintatica;*/
        /*print_tac($1->tac);*/
     }
;

programa:
    decl-global programa {
        if ($2 != NULL){
			$$ = $2;

			$$->tac = conecta_tacs($1->tac, $2->tac);
		}
        else{
			$$ = create_empty_node();
			$$->tac = $1->tac;
		}
    }
    | func programa {
        connect_nodes((comp_tree_t *)$1, (comp_tree_t *)$2);
        $$ = $1;

        if ($2 != NULL)
            $$->tac = conecta_tacs($1->tac, $2->tac);
        else
            $$->tac = conecta_tacs($1->tac, NULL);
    }
    | /* %empty */ {
        $$ = NULL;
    }
    | error {
        $$ = NULL;
    }
;

decl-global:
     tipo TK_IDENTIFICADOR ';' {

        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VARIAVEL, deslocamento_global);
        hash_item->escopo = EXTERNO;

        comp_tree_t* node_aux = create_empty_node();
        $$ = node_aux;

        deslocamento_global += tamanho_tipo(hash_item->type_var);

        node_aux->tac = (comp_list_tac_t*) criar_tac();
        $$->tac = (comp_list_tac_t*)criar_tac();
    }
    | tipo TK_IDENTIFICADOR '[' lista-expressao-vetor ']' ';' {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VETOR_INDEXADO, deslocamento_global);
        hash_item->escopo = EXTERNO;

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL, hash_item);
        /*node_identificador->next_brother = $4;*/
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);

        /*int operador = encontra_operador($4->hash->key);*/
        /*int tamanho;*/
        /*if(operador = USO_LITERAL){*/
            /*int tamanho_vetor = atoi($4->hash->key);*/
            /*tamanho = tamanho_vetor * tamanho_tipo(hash_item->type_var);*/
        /*}*/

        hash_item->list_args_vector = $4;
        /*$$->list_args = $4;*/
        /*hash_item->list_args = $4;*/
        /*hash_item->count_args = list_count($4);*/

        int dimensao = calcula_dimensao_arranjo($4);
        int tamanho = dimensao * tamanho_tipo(hash_item->type_var);

        deslocamento_global += tamanho;
        $$->tac = $4->tac;
    }
    | laco { yyerror("Não são permitidos laços fora do escopo de função"); }
    | condicional { yyerror("Não são permitidas expressões condicionais fora do escopo de função"); }
    | atribuicao { yyerror("Não são permitidas atribuições fora do escopo de função"); }
;

decl-local:
    tipo TK_IDENTIFICADOR {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VARIAVEL, symbol_table_cur->desloc);
        hash_item->escopo = INTERNO;

        comp_tree_t* node_aux = create_empty_node();

        $$ = node_aux;

        symbol_table_cur->desloc += tamanho_tipo(hash_item->type_var);
        node_aux->tac = (comp_list_tac_t*) criar_tac();
        $$->tac = (comp_list_tac_t*)criar_tac();

    }
    | tipo TK_IDENTIFICADOR '[' lista-expressao-vetor ']' {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VETOR_INDEXADO, symbol_table_cur->desloc);
        hash_item->escopo = INTERNO;

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $2, NULL, hash_item);
        /*node_identificador->next_brother = $4;*/
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);

        hash_item->list_args_vector = $4;
        /*$$->list_args = $4;*/
        /*hash_item->list_args = $4;*/
        /*hash_item->count_args = list_count($4);*/

        int dimensao = calcula_dimensao_arranjo($4);
        int tamanho = dimensao * tamanho_tipo(hash_item->type_var);

        symbol_table_cur->desloc += tamanho;

        $$->tac = $4->tac;
    }
;

decl-parametro:
    tipo TK_IDENTIFICADOR {
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_VARIAVEL, symbol_table_cur->desloc);

        $$ = list_create_item(hash_item);
    }
;

func:
    tipo TK_IDENTIFICADOR '('   {
                                    create_table(cur_dict_id++);
                                    linha_escopo_estatico = cur_line;
                                    tac_registro_ativacao = criar_tac_registro_ativacao(linha_escopo_estatico);
                                }
                                lista-parametros ')' corpo
                                {
                                    /*fprintf(stdout,"Deslocamento local: %d\n\n", symbol_table_cur->desloc); destroy_table(cur_dict_id--); */
                                    tac_fim_registro_ativacao = criar_tac_destroi_registro_ativacao($2);
                                }
    {
        /*hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_FUNCAO, 0);*/
        hash_item = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_FUNCAO, 0);

        /*$$ = create_node(IKS_AST_FUNCAO, $2, $7, hash_item);*/
        $$ = create_node(IKS_AST_FUNCAO, $2, $7, hash_item);

        /*list_func_connect($$, $4, hash_item);*/
        list_func_connect($$, $5, hash_item);
        /*verifica_return($$, $2, $1);*/
        verifica_return($$, $2, $1);

        /*$$->tac = (comp_list_tac_t*) criar_tac_funcao($2, $7->tac);*/
        $$->tac = (comp_list_tac_t*) criar_tac_funcao($2, $7->tac);

        /*print_table(symbol_table_cur);*/
        /*fprintf(stdout,"Deslocamento local: %d\n\n", symbol_table_cur->desloc);*/
    }
    | tipo TK_IDENTIFICADOR '(' ')' {
                                        hash_item_func = add_symbol(symbol_table_cur, $2, cur_line, TK_IDENTIFICADOR, $1, DECLARACAO_FUNCAO, 0);
                                        create_table(cur_dict_id++);
                                        linha_escopo_estatico = cur_line;
                                        tac_registro_ativacao = criar_tac_registro_ativacao(linha_escopo_estatico);
                                    }
                                    corpo
                                    {
                                        /*fprintf(stdout,"Deslocamento local: %d\n\n", symbol_table_cur->desloc); destroy_table(cur_dict_id--); */
                                        tac_fim_registro_ativacao = criar_tac_destroi_registro_ativacao($2);
                                    }
    {

        $$ = create_node(IKS_AST_FUNCAO, $2, $6, hash_item_func);

        hash_item_func->count_args = 0;
        verifica_return($$, $2, $1);

        /*$$->tac = criar_tac_funcao($2, $6->tac);*/
        // Conecta criação do registro de ativação antes da execução da função
        $$->tac = criar_tac_funcao($2, tac_registro_ativacao);

        /*fprintf(stdout, "tac_funcao\n");*/
        /*print_tac($$->tac);*/
        /*print_tac(tac_registro_ativacao);*/
        /*print_tac(tac_fim_registro_ativacao);*/
        /*fprintf(stdout, "tac_funcao_fim\n");*/

        // Cria tac para adicionar deslocamento ao sp

        conecta_bloco_ultimo_com_proximo($$->tac, $6->tac);
        conecta_tacs_irmaos($6->tac);

        conecta_bloco_ultimo_com_proximo($$->tac, tac_fim_registro_ativacao);
        conecta_tacs_irmaos(tac_fim_registro_ativacao);

    }
;

chamada-funcao:
    TK_IDENTIFICADOR '(' ')'
    {
        linha_escopo_dinamico = linha_escopo_estatico;
    }
    {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_FUNCAO, -1);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        $$ = create_node(IKS_AST_CHAMADA_DE_FUNCAO, $1, node_identificador, hash_item);

        hash_item->count_args = 0;
        verifica_argumentos($$, $1, 0);

        /*print_stack_dict(stack_scope);*/
        /*verifica_funcao($$, $1);*/

        $$->tac = (comp_list_tac_t*) criar_tac_chamada_funcao($1, NULL, cur_line+1, linha_escopo_dinamico);

        /*fprintf(stdout, "início tac chamada func\n");*/
        /*print_tac($$->tac);*/
        /*fprintf(stdout, "fim tac chamada func\n");*/
    }
    | TK_IDENTIFICADOR '(' lista-argumentos ')'
    {
        linha_escopo_dinamico = linha_escopo_estatico;
    }
    {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_FUNCAO, -1);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        $$ = create_node(IKS_AST_CHAMADA_DE_FUNCAO, NULL, node_identificador, NULL);

        list_func_connect($$, $3, hash_item);
        verifica_argumentos($$, $1, $3);

        $$->tac = (comp_list_tac_t*) criar_tac_chamada_funcao($1, NULL, cur_line+1, linha_escopo_dinamico);

        /*fprintf(stdout, "início tac chamada func\n");*/
        /*print_tac($$->tac);*/
        /*fprintf(stdout, "fim tac chamada func\n");*/
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
        comp_tree_t *node_aux = create_empty_node();
        node_aux->tac = criar_tac();
        $$ = node_aux;

        /*$$ = NULL;*/
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

        conecta_tacs($1->tac, $3->tac);
    }
    | comando ';'
    {
        $$ = $1;
    }
    /*| decl-local {
        $$ = $1;
    }
    | decl-local ';'{
        $$ = $1;
    }*/
    /*| decl-local ';' sequencia 
    {
        $$ = $3;

        conecta_tacs($1->tac, $3->tac);
    }*/
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

        $$->tac = criar_tac();
        conecta_tacs($$->tac, $3->tac);
    }
    | '{' { create_table(cur_dict_id++); } '}' { destroy_table(cur_dict_id--); } {
        $$ = create_node(IKS_AST_BLOCO, NULL, NULL, NULL);
    }
    | ';' {
        $$ = NULL;
    }
    | func {
        $$ = $1;
    }
    | decl-local {
        $$ = $1;
    }
;

condicional:
    TK_PR_IF '(' expressao ')' TK_PR_THEN comando {
        $3->next_brother = $6;
        $$ = create_node(IKS_AST_IF_ELSE, NULL, $3, NULL);
	$$->tac = cria_tac_if($3->tac, $6->tac);
    }
    | TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando {
        $3->next_brother = $6;
        $6->next_brother = $8;
        $$ = create_node(IKS_AST_IF_ELSE, NULL, $3, NULL);

	$$->tac = cria_tac_if_else($3->tac, $6->tac, $8->tac);
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
        $$->tac = cria_tac_while_do($3->tac, $6->tac);
  }
  | do comando while '(' expressao ')' {
        $2->next_brother = $5;
        $$ = create_node(IKS_AST_DO_WHILE, NULL, $2, NULL);
	
	$$->tac = cria_tac_do_while($5->tac, $2->tac);

  }
;

input:
    TK_PR_INPUT lista-expressao {
        $$ = create_node(IKS_AST_INPUT, NULL, $2, NULL);

        verifica_input($$);
    }
;

output:
    TK_PR_OUTPUT lista-expressao {
        $$ = create_node(IKS_AST_OUTPUT, NULL, $2, NULL);

        verifica_output($$);
    }
;

expressao:
    TK_IDENTIFICADOR {
        hash_item = verifica_uso_item(hash_function($1), USO_VARIAVEL, $1);
        //hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VARIAVEL, -1);
        $$ = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);

        $$->tac = criar_tac_literal(TK_IDENTIFICADOR, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, hash_item->desloc, NULL, NULL);
    }
    | literal {
        $$ = $1;
    }
    | chamada-funcao {
        $$ = $1;
    }
    | TK_IDENTIFICADOR '[' lista-expressao-dimensoes-vetor ']' {
        hash_item = verifica_uso_item(hash_function($1), USO_VETOR_INDEXADO, $1);

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);
        $$ = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);


        comp_list_tac_t* vetor_tacs_encadeado = cria_copia_conecta_tacs($3);
        comp_list_tac_t* vetor_tac = criar_tac_literal(TK_IDENTIFICADOR, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, hash_item->desloc, $3, hash_item->list_args_vector);
        $$->tac = conecta_tacs(vetor_tacs_encadeado, vetor_tac);
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

        $$->tac = criar_tac_expressao('!', $2->tac, NULL);
    }
    | '-' expressao %prec UMINUS {
        comp_tree_t* unario = create_node(IKS_AST_IDENTIFICADOR, "-", NULL, NULL);
        connect_nodes(unario, $2);
        $$ = unario;

        $$->tac = criar_tac_expressao(UMINUS, $2->tac, NULL);
    }
;

expressao-aritmetica:
    expressao '+' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_SOMA, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao('+', $1->tac, $3->tac);
    }
    | expressao '-' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_SUBTRACAO, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao('-', $1->tac, $3->tac);
    }
    | expressao '*' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_MULTIPLICACAO, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao('*', $1->tac, $3->tac);
    }
    | expressao '/' expressao 
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_ARIM_DIVISAO, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao('/', $1->tac, $3->tac);
    }
;

expressao-logica:
    expressao TK_OC_EQ expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_IGUAL, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao(TK_OC_EQ, $1->tac, $3->tac);
    }
    | expressao '<' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_L, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao('<', $1->tac, $3->tac);
    }
    | expressao TK_OC_LE expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_LE, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao(TK_OC_LE, $1->tac, $3->tac);
    }
    | expressao '>' expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_G, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao('>', $1->tac, $3->tac);
    }
    | expressao TK_OC_GE expressao 
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_GE, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao(TK_OC_GE, $1->tac, $3->tac);
    }
    | expressao TK_OC_NE expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_COMP_DIF, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        $$->tac = criar_tac_expressao(TK_OC_NE, $1->tac, $3->tac);
    }
    | expressao TK_OC_AND expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_E, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        //$$->tac = criar_tac_expressao(TK_OC_AND, $1->tac, $3->tac);
        $$->tac = criar_tac_expressao_logica(TK_OC_AND, $1->tac, $3->tac);
    }
    | expressao TK_OC_OR expressao
    {
        $1->next_brother = $3;
        $$ = create_node(IKS_AST_LOGICO_OU, NULL, $1, NULL);
        verifica_coersao_arvore($$, $1, $3);

        //$$->tac = criar_tac_expressao(TK_OC_OR, $1->tac, $3->tac);
        $$->tac = criar_tac_expressao_logica(TK_OC_OR, $1->tac, $3->tac);
    }
;

atribuicao:
    TK_IDENTIFICADOR '=' expressao
    {
        //hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VARIAVEL, -1);
        hash_item = verifica_uso_item(hash_function($1), USO_VARIAVEL, $1);

        int tipo = encontra_tipo($1,DECLARACAO_VARIAVEL);
        if(tipo == IKS_TYPE_NOT_DEFINED){
            exit(IKS_TYPE_NOT_DEFINED);
        }

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, $3, hash_item);

        $$ = create_node(IKS_AST_ATRIBUICAO, NULL, node_identificador, NULL);
        verifica_atribuicao($3,tipo);

        /*fprintf(stdout, "entrou tac atribuicao key %s escopo %d\n", hash_item->key, hash_item->escopo);*/
        $$->tac = (comp_list_tac_t*) criar_tac_atribuicao($1, $3->tac, hash_item->desloc, hash_item->escopo);
    }
    | TK_IDENTIFICADOR '[' lista-expressao-dimensoes-vetor ']' '=' expressao
    {
        hash_item = verifica_uso_item(hash_function($1), USO_VETOR_INDEXADO, $1);

        int tipo = encontra_tipo($1,DECLARACAO_VETOR_INDEXADO);
        if(tipo == IKS_TYPE_NOT_DEFINED){
            exit(IKS_TYPE_NOT_DEFINED);
        }

        comp_tree_t* node_identificador = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);

        comp_tree_t* vetor = create_node(IKS_AST_VETOR_INDEXADO, NULL, node_identificador, NULL);
        vetor->next_brother = $6;

        $$ = create_node(IKS_AST_ATRIBUICAO, NULL, vetor, NULL);
        /*verifica_tipo_indexador($3);*/
        verifica_atribuicao($6,tipo);

        // Cria lista encadeada dos tacs da lista de parâmetros
        comp_list_tac_t* vetor_tacs_encadeado = cria_copia_conecta_tacs($3);
        comp_list_tac_t* vetor_tac = criar_tac_atribuicao_vetor($1, $6->tac, $3, hash_item->list_args_vector, hash_item->desloc, tamanho_tipo(hash_item->type_var), hash_item->escopo);
        $$->tac = conecta_tacs(vetor_tacs_encadeado, vetor_tac);
    }
;

literal:
    TK_LIT_CHAR {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_CHAR, IKS_CHAR, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
        $$->tac = criar_tac_literal(TK_LIT_CHAR, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);
    }
    | TK_LIT_FALSE {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_BOOL, IKS_BOOL, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
        $$->tac = criar_tac_literal(TK_LIT_FALSE, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);
    }
    | TK_LIT_FLOAT {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_FLOAT, IKS_FLOAT, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
        $$->tac = criar_tac_literal(TK_LIT_FLOAT, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);
    }
    | TK_LIT_INT {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_INT, IKS_INT, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
        $$->tac = criar_tac_literal(TK_LIT_INT, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);
    }
    | TK_LIT_STRING {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_STRING, IKS_STRING, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
        $$->tac = criar_tac_literal(TK_LIT_STRING, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);
    }
    | TK_LIT_TRUE {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_BOOL, IKS_BOOL, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_LITERAL, $1, NULL, hash_item);
        $$->tac = criar_tac_literal(TK_LIT_TRUE, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);
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
        hash_item = add_symbol(symbol_table_cur, "return", cur_line, TK_PR_RETURN, IKS_TYPE_NOT_DEFINED, USO_LITERAL, -1);

        $$ = create_node(IKS_AST_RETURN, NULL, $2, hash_item);
    }
;

expressao-parametro-chamada-funcao:
    TK_IDENTIFICADOR {
        hash_item = verifica_uso_item(hash_function($1), USO_VARIAVEL, $1);
        //hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_IDENTIFICADOR, IKS_TYPE_NOT_DEFINED, USO_VARIAVEL, -1);
        $$ = create_node(IKS_AST_IDENTIFICADOR, $1, NULL, hash_item);

        $$->tac = criar_tac_literal(TK_IDENTIFICADOR, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, hash_item->desloc, NULL, NULL);
    }
    | literal {
        $$ = $1;
    }
    | chamada-funcao {
        $$ = $1;
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

        $$->tac = criar_tac_expressao('!', $2->tac, NULL);
    }
    | '-' expressao %prec UMINUS {
        comp_tree_t* unario = create_node(IKS_AST_IDENTIFICADOR, "-", NULL, NULL);
        connect_nodes(unario, $2);
        $$ = unario;

        $$->tac = criar_tac_expressao(UMINUS, $2->tac, NULL);
    }

;

/*lista-expressao-parametro-chamada-funcao:
	expressao-parametro-chamada-funcao {
		$$ = $1;	
	}
	|expressao-parametro-chamada-funcao ',' lista-expressao-parametro-chamada-funcao{
		connect_nodes($1, $3);
        $$ = $1;
	}
;*/

lista-argumentos:
    expressao-parametro-chamada-funcao {
        $$ = list_create_item($1->hash);
    }
    | expressao-parametro-chamada-funcao ',' lista-argumentos
    {
        $$ = list_concat(list_create_item($1->hash), $3);
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


lista-parametros:
    decl-parametro {
        $$ = $1;
    }
    | decl-parametro ',' lista-parametros
    {
        $$ = list_concat($1, $3);
    }
;

lista-expressao-vetor:
    TK_LIT_INT {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_INT, IKS_INT, USO_LITERAL, -1);
        $$->tac = criar_tac_literal(TK_LIT_INT, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);

        $$ = list_tac_create_item($$->tac);
        $$->dimensao = atoi(hash_item->key);
    }
    | TK_LIT_INT ',' lista-expressao-vetor
    {
        hash_item = add_symbol(symbol_table_cur, $1, cur_line, TK_PR_INT, IKS_INT, USO_LITERAL, -1);
        $$->tac = criar_tac_literal(TK_LIT_INT, hash_item->type_var, tamanho_tipo(hash_item->type_var), hash_item->key, hash_item->escopo, -1, NULL, NULL);

        comp_list_tac_vector_t* tac_item = list_tac_create_item($$->tac);
        tac_item->dimensao = atoi(hash_item->key);

        $$ = list_tac_concat(tac_item, $3);

        conecta_tacs($$->tac, $3->tac);
    }
;

lista-expressao-dimensoes-vetor:
    expressao {
        $$ = list_tac_create_item($1->tac);
    }
    | expressao ',' lista-expressao-dimensoes-vetor
    {
        $$ = list_tac_concat(list_tac_create_item($1->tac), $3);
    }
;

%%
