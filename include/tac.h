#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "comp_tree.h"

typedef struct comp_list_tac_t {
	struct comp_list_tac_t *tac_previous;
	struct comp_list_tac_t *tac_next;
	int tipo;
	char valor1[1000], valor2[1000], valor3[1000];
}comp_list_tac_t;


void print_tac (comp_list_tac_t *tac);