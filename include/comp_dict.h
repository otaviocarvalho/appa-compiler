// Estruturas do dicionário
typedef struct comp_dict_item_t comp_dict_item_t;
struct comp_dict_item_t {
    char *key;
    char *value;
};

typedef struct comp_dict_t comp_dict_t;
struct comp_dict_t {
    comp_dict_item_t item;
    comp_dict_t *next;
};

// Variável global do escopo corrente
comp_dict_t* symbol_table_cur_scope;

// Diretivas externas
extern char* yytext;
