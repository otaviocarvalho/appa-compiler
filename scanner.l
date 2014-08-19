/*
  Grupo Appa - Edson Andrade | Otávio Carvalho | Lisandro Procedi
*/
%{
#include "parser.h" //arquivo automaticamente gerado pelo bison
#include <stdlib.h>
#include <stdio.h>

extern int cur_line;
extern struct comp_dict_t* symbol_table_cur_scope;

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

"true"      { return TK_LIT_TRUE; }
"false"     { return TK_LIT_FALSE; }

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

({LCASE}|{UCASE}|{UNDERSCORE})+(({LCASE}|{UCASE}|{UNDERSCORE})|{DIGIT})* { return TK_IDENTIFICADOR; }

-?{DIGIT}+              { add_symbol(symbol_table_cur_scope, yytext, TK_LIT_INT); return TK_LIT_INT; }
-?{DIGIT}+"."{DIGIT}+   { add_symbol(symbol_table_cur_scope, yytext, TK_LIT_INT); return TK_LIT_FLOAT; }
\'.\'                   { add_symbol(symbol_table_cur_scope, yytext, TK_LIT_INT); return TK_LIT_CHAR; }
\"[^"]*\"               { add_symbol(symbol_table_cur_scope, yytext, TK_LIT_INT); return TK_LIT_STRING;}
" "        {  }
"\t"       {  }
\n         { cur_line++; }
.          { return TOKEN_ERRO; }

%%
