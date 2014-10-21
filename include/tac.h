#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "comp_tree.h"

struct lista_tac {
	LISTA_TAC *tac_previous;
	LISTA_TAC *tac_next;
	int tipo;
	char valor1[1000], valor2[1000], valor3[1000];
};
