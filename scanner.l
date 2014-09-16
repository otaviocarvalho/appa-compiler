/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include "parser.h" //arquivo automaticamente gerado pelo bison
#include "comp_dict.h"
#include <stdlib.h>
#include <stdio.h>

extern int cur_line;
extern struct comp_dict_t* symbol_table_root;
extern struct comp_dict_t* symbol_table_cur;

%}

%x comment
%x string

DIGIT [0-9]
LCASE [a-z]
UCASE [A-Z]
UNDERSCORE [_]

%%

"int"       { return TK_PR_INT; } ;
"float"     { return TK_PR_FLOAT; } ;
"bool"      { return TK_PR_BOOL; } ;
"char"      { return TK_PR_CHAR; } ;
"string"    { return TK_PR_STRING; } ;
"if"        { return TK_PR_IF; } ;
"then"      { return TK_PR_THEN; } ;
"else"      { return TK_PR_ELSE; } ;
"while"     { return TK_PR_WHILE; } ;
"do"        { return TK_PR_DO; } ;
"input"     { return TK_PR_INPUT; } ;
"output"    { return TK_PR_OUTPUT; } ;
"return"    { return TK_PR_RETURN; } ;

"true"      {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_PR_BOOL); 
    yylval.symbol_name = strdup(yytext);
    return TK_LIT_TRUE;
}
"false"     {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_PR_BOOL); 
    yylval.symbol_name = strdup(yytext);
    return TK_LIT_FALSE;
}


"//".* { }
"/*"    BEGIN(comment);
<comment>[^*\n]*
<comment>"*"+[^*/\n]*
<comment>\n         cur_line++;
<comment>"*"+"/"    BEGIN(INITIAL);

","|";"|":"|")"|"("|"["|"]"|"{"|"}"|"+"|"-"|"*"|"/"|"<"|">"|"="|"!"|"&"|"$"  { return yytext[0]; }

"<="    { return TK_OC_LE; }
">="    { return TK_OC_GE; }
"=="    { return TK_OC_EQ; }
"!="    { return TK_OC_NE; }
"&&"    { return TK_OC_AND; }
"||"    { return TK_OC_OR; }

({LCASE}|{UCASE}|{UNDERSCORE})+(({LCASE}|{UCASE}|{UNDERSCORE})|{DIGIT})* {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_IDENTIFICADOR);
    yylval.symbol_name = strdup(yytext);
    return TK_IDENTIFICADOR;
}

{DIGIT}+              {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_PR_INT);
    yylval.symbol_name = strdup(yytext);
    return TK_LIT_INT;
}
{DIGIT}+"."{DIGIT}+   {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_PR_FLOAT);
    yylval.symbol_name = strdup(yytext);
    return TK_LIT_FLOAT;
}
\'.\'                 {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_PR_CHAR);
    yylval.symbol_name = strdup(yytext);
    return TK_LIT_CHAR;
}
\"[^"]*\"             {
    yylval.symbol = add_symbol(symbol_table_cur, yytext, cur_line, TK_PR_STRING);
    yylval.symbol_name = strdup(yytext);
    return TK_LIT_STRING;
}

" "        {  }
"\t"       {  }
\n         { cur_line++; }
.          { return TOKEN_ERRO; }

%%
