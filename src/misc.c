#include "misc.h"

int getLineNumber (void)
{
    return cur_line;
}

void yyerror (char const *mensagem)
{
    fprintf (stderr, "%s (line %d)\n", mensagem, cur_line);
}

void main_init (int argc, char **argv)
{
    hcreate(HASH_SIZE);
}

void main_finalize (void)
{

}
