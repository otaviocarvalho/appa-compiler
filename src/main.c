#include "main.h"
#include "tac.h"

extern FILE *yyin;
extern char *yytext;
extern int getLineNumber();
#define print_nome(TOKEN) printf("%d " #TOKEN " [%s]\n", getLineNumber(), yytext);
#define print_nome2(TOKEN) printf("%d %c\n", getLineNumber(), TOKEN);
#define USER_INIT main_init (argc, argv);
#define USER_FINALIZE main_finalize ();

// Globally defined optimization heuristics
int global_optimization_parameter = 0;

int main_avaliacao_etapa_1 (int argc, char **argv)
{
  int token = 0;
  while (token = yylex()) {
    switch (token){
    case ',':
    case ';':
    case ':':
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '+':
    case '-':
    case '*':
    case '/':
    case '<':
    case '>':
    case '=':
    case '!':
    case '&':
    case '$': print_nome2 (token); break;
    case TK_PR_INT: print_nome(TK_PR_INT); break;
    case TK_PR_FLOAT: print_nome(TK_PR_FLOAT); break;
    case TK_PR_BOOL: print_nome (TK_PR_BOOL); break;
    case TK_PR_CHAR: print_nome (TK_PR_CHAR); break;
    case TK_PR_STRING: print_nome (TK_PR_STRING); break;
    case TK_PR_IF: print_nome (TK_PR_IF); break;
    case TK_PR_THEN: print_nome (TK_PR_THEN); break;
    case TK_PR_ELSE: print_nome (TK_PR_ELSE); break;
    case TK_PR_WHILE: print_nome (TK_PR_WHILE); break;
    case TK_PR_DO: print_nome (TK_PR_DO); break;
    case TK_PR_INPUT: print_nome (TK_PR_INPUT); break;
    case TK_PR_OUTPUT: print_nome (TK_PR_OUTPUT); break;
    case TK_PR_RETURN: print_nome (TK_PR_RETURN); break;
    case TK_OC_LE: print_nome (TK_OC_LE); break;
    case TK_OC_GE: print_nome (TK_OC_GE); break;
    case TK_OC_EQ: print_nome (TK_OC_EQ); break;
    case TK_OC_NE: print_nome (TK_OC_NE); break;
    case TK_OC_AND: print_nome (TK_OC_AND); break;
    case TK_OC_OR: print_nome (TK_OC_OR); break;
    case TK_LIT_INT: print_nome (TK_LIT_INT); break;
    case TK_LIT_FLOAT: print_nome (TK_LIT_FLOAT); break;
    case TK_LIT_FALSE: print_nome (TK_LIT_FALSE); break;
    case TK_LIT_TRUE: print_nome (TK_LIT_TRUE); break;
    case TK_LIT_CHAR: print_nome (TK_LIT_CHAR); break;
    case TK_LIT_STRING: print_nome (TK_LIT_STRING); break;
    case TK_IDENTIFICADOR: print_nome (TK_IDENTIFICADOR); break;
    case TOKEN_ERRO:  print_nome (TOKEN_ERRO); break;
    default: printf ("<Invalid Token with code %d>\n", token); return 1; break;
    }
  }
  return 0;
}

int main_avaliacao_etapa_2 (int argc, char **argv)
{
  int ret = yyparse();
  return ret;
}

int main_avaliacao_etapa_2_plus (int argc, char **argv, int file, int optimization_type)
{
    global_optimization_parameter = optimization_type;

    if (file == 1){
        comp_list_tac_t* list_tacs = gerar_tacs_input(yyin);
        print_tac(list_tacs);
    }
    else {
        int ret = yyparse();
        return ret;
    }

    return 0;
}

int main_avaliacao_etapa_3 (int argc, char **argv)
{
  gv_init(NULL);
  int ret = yyparse();
  gv_close();
  return ret;
}

int main_avaliacao_etapa_4 (int argc, char **argv)
{
  return main_avaliacao_etapa_2 (argc, argv);
}

int main_avaliacao_etapa_5 (int argc, char **argv)
{
  return main_avaliacao_etapa_2 (argc, argv);
}

int main_avaliacao_etapa_6 (int argc, char **argv, int file, int optimization_type)
{
  return main_avaliacao_etapa_2_plus (argc, argv, file, optimization_type);
}

int main_avaliacao_etapa_7 (int argc, char **argv, int file, int optimization_type)
{
    return main_avaliacao_etapa_2_plus (argc, argv, file, optimization_type);
}

int main (int argc, char **argv)
{
  int flag_file = 0;
  int optimization_type = 0;
  //if some argument is provided, treat it as input
  if (argc != 1) {
    //process input parameters
    if (argc == 2) {
        if (strncmp(argv[1], "-O", 2) == 0) {
            optimization_type = atoi(argv[1]+2);
        }
        else {
            optimization_type = 0;
            yyin = fopen(argv[1], "r");
        }
    }
    else if (argc == 3){
        optimization_type = 1;
        if (strncmp(argv[1], "-O", 2) == 0) {
            optimization_type = atoi(argv[1]+2);
        }
        yyin = fopen(argv[2], "r");
    }
    else {
        fprintf(stdout, "Input format: ./main [-O<optimization-type-number>] <input file>\n");
        yyin = fopen(argv[1], "r");
    }

    //if fopen fails, yyin continues to be stdin
    if (yyin == NULL) {
      yyin = stdin;
    }
    else {
        flag_file = 1;
    }
  }
  USER_INIT;
  int r;
#ifdef AVALIACAO_ETAPA_1
  r = main_avaliacao_etapa_1 (argc, argv);
#elif AVALIACAO_ETAPA_2
  r = main_avaliacao_etapa_2 (argc, argv);
#elif AVALIACAO_ETAPA_3
  r = main_avaliacao_etapa_3 (argc, argv);
#elif AVALIACAO_ETAPA_4
  r = main_avaliacao_etapa_4 (argc, argv);
#elif AVALIACAO_ETAPA_5
  r = main_avaliacao_etapa_5 (argc, argv);
#elif AVALIACAO_ETAPA_6
  r = main_avaliacao_etapa_6 (argc, argv, flag_file, optimization_type);
#elif AVALIACAO_ETAPA_7
  r = main_avaliacao_etapa_7 (argc, argv, flag_file, optimization_type);
#else
  r = 0;
#endif
  USER_FINALIZE;
  return r;
}
