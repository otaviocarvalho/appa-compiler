#include "tac.h"

comp_list_tac_t* tac_label_prox;

int registrador_atual = 0;
int label_atual = 0;

void print_tac(comp_list_tac_t* raiz){
    comp_list_tac_t* aux = raiz;
    if (raiz == NULL) {
        printf("RAIZ NULL\n");
    }

    while (aux != NULL){
        print_tac_item(aux);
        aux = aux->tac_next;
    }
}

void print_tac_item(comp_list_tac_t* tac){
    switch(tac->tipo){
        case '+':
            printf("add %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '-':
            printf("sub %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '/':
            printf("div %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '*':
            printf("mult %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case UMINUS:
            printf("sub 0, %s => %s\n", tac->v2, tac->v1);
            break;
        case TK_OC_LE:
            printf("cmp_LE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_GE:
            printf("cmp_GE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_EQ:
            printf("cmp_EQ %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_NE:
            printf("cmp_NE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_AND:
            printf("and %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_OR:
            printf("or %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '<':
            printf("cmp_LT %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '>':
            printf("cmp_GT %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TAC_ATRIBUICAO:
            printf("store %s => %s\n", tac->v2, tac->v3);
            break;
        case TAC_LABEL:
            printf("%s: \n", tac->v1);
            break;
        case TAC_LOAD_VAL:
            printf("loadI %s => %s \n", tac->v3, tac->v1);
            break;
        case TAC_LOAD:
            printf("load %s => %s \n", tac->v3, tac->v1);
            break;
        case TAC_CBR:
            printf("cbr %s -> %s, %s \n", tac->v1, tac->v2, tac->v3);
            break;
		case TAC_JUMP_LABEL:
            printf("jumpI -> %s\n", tac->v1);
			break;
		case TAC_ADD_VAL:
            printf("addI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TAC_MULT_VAL:
            printf("multI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
		case TAC_NOP:
			printf("nop\n");
			break;
		default:
            break;
    }
}

void conecta_tacs_irmaos(comp_list_tac_t* last){
    comp_list_tac_t* aux = last;
    while (aux != NULL){
        if (aux->tac_prev != NULL)
            aux->tac_prev->tac_next = aux;
        aux = aux->tac_prev;
    }
}

void conecta_bloco_ultimo_com_proximo(comp_list_tac_t* bloco_tac, comp_list_tac_t* ultimo){
    comp_list_tac_t* tac_block_aux = bloco_tac;
    while (tac_block_aux != NULL){
        if (tac_block_aux->tac_next == NULL){
		ultimo->tac_prev = tac_block_aux;
	}
	tac_block_aux = tac_block_aux->tac_next;
    }
}

comp_list_tac_t* busca_bloco_ultimo(comp_list_tac_t* bloco){
	if (bloco == NULL)
		return NULL;

	comp_list_tac_t* tac_block_aux = bloco;
    while (tac_block_aux->tac_next != NULL){
		tac_block_aux = tac_block_aux->tac_next;
    }

    return tac_block_aux;
}

comp_list_tac_t* conecta_tacs(comp_list_tac_t* tac1, comp_list_tac_t* tac2){
    comp_list_tac_t* aux_tac1;
    comp_list_tac_t* aux_tac2;
    int i = 0;

    if(tac1 != NULL){
        if(tac2 != NULL){
            aux_tac1 = tac1;
            // Percorre até o fim da lista de tacs
            while (aux_tac1->tac_next != NULL){
                aux_tac1 = aux_tac1->tac_next;
            }

            aux_tac2 = tac2;
            // Percorre até o início da lista de tacs
            while (aux_tac2->tac_prev != NULL){
                aux_tac2 = aux_tac2->tac_prev;
            }

            aux_tac1->tac_next = aux_tac2;
            aux_tac2->tac_prev = aux_tac1;
        }

        return tac1;
    }
    else {
        if(tac2 != NULL){
            return tac2;
        }
    }

    return NULL;
}

char *criar_registrador()
{
    char *new_registrador = (char *) malloc (100 * sizeof(char));
    sprintf(new_registrador,"r%d", registrador_atual);
    registrador_atual++;
    return new_registrador;
}

char *criar_label()
{
    char *new_label = (char *) malloc (100 * sizeof(char));
    sprintf(new_label, "L%d", label_atual);
    label_atual++;
    return new_label;
}

comp_list_tac_t *montar_tac(int tipo, char* valor1, char* valor2, char* valor3)
{
    comp_list_tac_t* new_tac = criar_tac();
    if (new_tac == NULL)
    {
        printf("Erro na função montar_tac\n\n");
        exit(-1);
    }

    new_tac->tipo = tipo;
    if (valor1 != NULL)
        strcpy(new_tac->v1, valor1);
    if (valor2 != NULL)
        strcpy(new_tac->v2, valor2);
    if (valor3 != NULL)
        strcpy(new_tac->v3, valor3);

    return new_tac;
}

comp_list_tac_t *criar_tac_expressao(int operacao, comp_list_tac_t *tac1, comp_list_tac_t *tac2) {
    comp_list_tac_t *new_tac = criar_tac();

    if (tac1 == NULL)
        new_tac = montar_tac(operacao, criar_registrador(), "0", tac2->v1);
    else if (tac2 == NULL)
        new_tac = montar_tac(operacao, criar_registrador(), tac1->v1, "0");
    else {
		new_tac = montar_tac(operacao, criar_registrador(), busca_bloco_ultimo(tac1)->v1, busca_bloco_ultimo(tac2)->v1);
	}

    if (tac1 != NULL) {
        if (tac2 != NULL) {
            tac2->tac_prev = busca_bloco_ultimo(tac1);
            new_tac->tac_prev = busca_bloco_ultimo(tac2);

			conecta_tacs_irmaos(new_tac);
        }
        else {
            new_tac->tac_prev = busca_bloco_ultimo(tac1);
        }
    }

    return tac1;
}

comp_list_tac_t *criar_tac_literal(int tipo_literal, int tipo, int tamanho_tipo, char* valor, int escopo, int desloc, comp_list_tac_vector_t* lista_dimensoes)
{
	comp_list_tac_t *new_tac = criar_tac();
	comp_list_tac_t *tac_desloc = criar_tac();

	// Cria referência para o rbss
	char *escopo_str = (char *) malloc (100 * sizeof(char));
    if (escopo == EXTERNO) {
        strcpy(escopo_str,"rbss");
    }
    else {
        strcpy(escopo_str,"rarp");
    }

	/*char *rbss = (char *) malloc (100 * sizeof(char));*/
    /*strcpy(rbss,"rbss");*/

	/*char *rarp = (char *) malloc (100 * sizeof(char));*/
    /*strcpy(rarp,"rarp");*/

	char *desloc_str = (char *) malloc (100 * sizeof(char));
	char *reg_mult_str = (char *) malloc (100 * sizeof(char));
	char *tamanho_tipo_str = (char *) malloc (100 * sizeof(char));
	char *reg_desloc = (char *) malloc (100 * sizeof(char));

	switch(tipo_literal){
		case TK_LIT_STRING:
		case TK_LIT_CHAR:
		case TK_LIT_TRUE:
		case TK_LIT_FALSE:
		case TK_LIT_INT:
		case TK_LIT_FLOAT:
			new_tac = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, valor);
			conecta_tacs_irmaos(new_tac);
			break;
		case TK_IDENTIFICADOR:
            if (lista_dimensoes != NULL){
                strcpy(reg_desloc, criar_registrador());
                strcpy(reg_mult_str, criar_registrador());

                sprintf(desloc_str,"%d",desloc);
                sprintf(tamanho_tipo_str,"%d",tamanho_tipo);

                comp_list_tac_t* tac_mult_tipo_desloc = montar_tac(TAC_MULT_VAL, reg_mult_str, lista_dimensoes->tac->v1, tamanho_tipo_str);
                comp_list_tac_t* tac_load_reg_desloc = montar_tac(TAC_ADD_VAL, reg_desloc, tac_mult_tipo_desloc->v1, desloc_str);
                tac_desloc = montar_tac('+', criar_registrador(), escopo_str, tac_load_reg_desloc->v1);
                new_tac = montar_tac(TAC_LOAD, criar_registrador(), NULL, tac_desloc->v1);

                tac_mult_tipo_desloc->tac_prev = lista_dimensoes->tac;
                tac_load_reg_desloc->tac_prev = tac_mult_tipo_desloc;
                tac_desloc->tac_prev = tac_load_reg_desloc;
                new_tac->tac_prev = tac_desloc;

                conecta_tacs_irmaos(new_tac);

                return lista_dimensoes->tac;
            }
            else {
                sprintf(desloc_str,"%d",desloc);
                char* reg_desloc = criar_registrador();
                tac_desloc = montar_tac(TAC_ADD_VAL, reg_desloc, escopo_str, desloc_str);
                new_tac = montar_tac(TAC_LOAD, criar_registrador(), NULL, reg_desloc);

                new_tac->tac_prev = tac_desloc;
                conecta_tacs_irmaos(new_tac);

                return tac_desloc;
            }

			/*if (escopo == INTERNO) {*/
/*// 				sprintf(desloc_str,"%d",desloc);*/
/*// 				new_tac = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, valor);*/
/*// 				conecta_tacs_irmaos(new_tac);*/
				/*sprintf(desloc_str,"%d",desloc);*/
				/*char* reg_desloc = criar_registrador();*/
				/*tac_desloc = montar_tac(TAC_ADD_VAL, reg_desloc, rarp, desloc_str);*/
				/*new_tac = montar_tac(TAC_LOAD, criar_registrador(), NULL, reg_desloc);*/

				/*new_tac->tac_prev = tac_desloc;*/
				/*conecta_tacs_irmaos(new_tac);*/

				/*return tac_desloc;*/
			/*}*/
			/*else if (escopo == EXTERNO) {*/
				/*sprintf(desloc_str,"%d",desloc);*/
				/*char* reg_desloc = criar_registrador();*/
				/*tac_desloc = montar_tac(TAC_ADD_VAL, reg_desloc, rbss, desloc_str);*/
				/*new_tac = montar_tac(TAC_LOAD, criar_registrador(), NULL, reg_desloc);*/

				/*new_tac->tac_prev = tac_desloc;*/
				/*conecta_tacs_irmaos(new_tac);*/

				/*return tac_desloc;*/
			/*}*/

			break;
	}

	return new_tac;
}

comp_list_tac_t* criar_tac(){
    comp_list_tac_t* new_tac = (comp_list_tac_t*) malloc(sizeof(comp_list_tac_t));
    new_tac->tipo = -1;
    new_tac->v1[0] = '\0';
    new_tac->v2[0] = '\0';
    new_tac->v3[0] = '\0';
    new_tac->tac_next = NULL;
    new_tac->tac_prev = NULL;

    return new_tac;
}

comp_list_tac_t* criar_tac_funcao(char *id, comp_list_tac_t* tac_func) {
    comp_list_tac_t* tac_rotulo = montar_tac(TAC_LABEL, id, NULL, NULL);

    if(tac_func !=NULL){
        tac_func->tac_prev = tac_rotulo;
        conecta_tacs_irmaos(tac_func);
        return tac_func;
    }

    return tac_rotulo;
}

// Preencher
comp_list_tac_t* criar_tac_chamada_funcao(char* id, comp_list_tac_t* tac_func) {
    comp_list_tac_t* tac_new = criar_tac();

    return tac_new;
}

comp_list_tac_t* criar_tac_atribuicao(char *dest, comp_list_tac_t* orig, int desloc, int escopo) {
    comp_list_tac_t* tac_atr = criar_tac();
    comp_list_tac_t* tac_load_val = criar_tac();
    comp_list_tac_t* tac_load_desloc = criar_tac();
    char *desloc_str = (char *) malloc (100 * sizeof(char));
	char *rbss = (char *) malloc (100 * sizeof(char));

	strcpy(rbss, "rbss");
	sprintf(desloc_str,"%d",desloc);

	// Calcula o deslocamento com base no escopo da variável
	if (escopo == INTERNO) {
		tac_load_desloc = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, desloc_str);
		tac_load_desloc->tac_prev = tac_load_val;
	}
	else if (escopo == EXTERNO) {
		tac_load_desloc = montar_tac(TAC_ADD_VAL, criar_registrador(), rbss, desloc_str);

		conecta_bloco_ultimo_com_proximo(orig, tac_load_desloc);
	}

	tac_atr = montar_tac(TAC_ATRIBUICAO, NULL, (busca_bloco_ultimo(orig))->v1, tac_load_desloc->v1);
	tac_atr->tac_prev = tac_load_desloc;

    // Conecta do último tac até o primeiro na ordem inversa
    conecta_tacs_irmaos(tac_atr);

	return orig;
}

comp_list_tac_t* calcula_tac_lista_param_desloc(comp_list_tac_vector_t* lista_parametros, char* desloc, int tamanho_tipo, int escopo){
    comp_list_tac_t* tac_soma_desloc = criar_tac();
    comp_list_tac_t* tac_soma_desloc_aux = criar_tac();
    comp_list_tac_t* tac_soma_desloc_escopo = criar_tac();

    char *tamanho_str = (char *) malloc (100 * sizeof(char));
    char *reg_escopo = (char *) malloc (100 * sizeof(char));
    char *reg_mult_desloc_vetor = (char *) malloc (100 * sizeof(char));

    sprintf(tamanho_str,"%d",tamanho_tipo);

    if (escopo == EXTERNO){
        strcpy(reg_escopo, "rbss");
    }
    else {
        strcpy(reg_escopo, "rarp");
    }

    // Percorrer lista parametros somando deslocamentos
/*    int criou_primeiro_tac = 0;*/
    /*comp_list_tac_vector_t* aux = lista_parametros;*/
    /*while (aux != NULL){*/
        /*comp_list_tac_t* aux_bloco_ultimo = busca_bloco_ultimo(aux->tac);*/

        /*if (!criou_primeiro_tac) {*/
            /*tac_soma_desloc = montar_tac(TAC_ADD_VAL, criar_registrador(), aux_bloco_ultimo->v1, desloc);*/
            /*criou_primeiro_tac = 1;*/
        /*}*/
        /*else {*/
            /*tac_soma_desloc = montar_tac('+', criar_registrador(), aux_bloco_ultimo->v1, tac_soma_desloc->v1);*/
        /*}*/

        /*aux_bloco_ultimo->tac_prev = tac_soma_desloc;*/
        /*conecta_tacs_irmaos(aux_bloco_ultimo);*/

        /*aux = aux->next;*/
    /*}*/

    comp_list_tac_vector_t* aux = lista_parametros;
    if (aux != NULL) {
        comp_list_tac_t* aux_bloco_ultimo = busca_bloco_ultimo(aux->tac);

        strcpy(reg_mult_desloc_vetor, criar_registrador());
        tac_soma_desloc = montar_tac(TAC_MULT_VAL, reg_mult_desloc_vetor, aux_bloco_ultimo->v1, tamanho_str);

        comp_list_tac_t* tac_soma_desloc_escopo = montar_tac('+', criar_registrador(), reg_escopo, tac_soma_desloc->v1);

        tac_soma_desloc->tac_prev = aux_bloco_ultimo;
        tac_soma_desloc_escopo->tac_prev = tac_soma_desloc;

        conecta_tacs_irmaos(tac_soma_desloc_escopo);

        // Define o TAC para o retorno do bloco de comandos criado
        tac_soma_desloc = aux->tac;
    }

    return tac_soma_desloc;
}

comp_list_tac_t* criar_tac_atribuicao_vetor(char *dest, comp_list_tac_t* orig, comp_list_tac_vector_t* lista_parametros, int desloc, int tamanho_tipo, int escopo) {
    comp_list_tac_t* tac_atr = criar_tac();
    comp_list_tac_t* tac_load_val = criar_tac();
    comp_list_tac_t* tac_load_desloc = criar_tac();
    comp_list_tac_t* desloc_vetor = criar_tac();

    char *desloc_str = (char *) malloc (100 * sizeof(char));
    char *rbss = (char *) malloc (100 * sizeof(char));

    strcpy(rbss, "rbss");
    sprintf(desloc_str,"%d",desloc);

    // Calcula o deslocamento com base no escopo da variável
    if (escopo == INTERNO) {
        tac_load_desloc = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, desloc_str);
        tac_load_desloc->tac_prev = tac_load_val;
    }
    else if (escopo == EXTERNO) {
        // Calcula deslocamento com relação ao vetor
        desloc_vetor = calcula_tac_lista_param_desloc(lista_parametros, desloc_str, tamanho_tipo, escopo);
        conecta_bloco_ultimo_com_proximo(orig, desloc_vetor);
    }

    tac_atr = montar_tac(TAC_ATRIBUICAO, NULL, (busca_bloco_ultimo(orig))->v1, (busca_bloco_ultimo(desloc_vetor))->v1);
    tac_atr->tac_prev = busca_bloco_ultimo(desloc_vetor);

    // Conecta do último tac até o primeiro na ordem inversa
    conecta_tacs_irmaos(tac_atr);

    return orig;
}

comp_list_tac_t *cria_tac_if(comp_list_tac_t *condicional, comp_list_tac_t *bloco_if){
    char rotulo_if[100], rotulo_continue[100];
    comp_list_tac_t *tac_jump, *tac_rotulo_if, *tac_rotulo_continue;
    strcpy(rotulo_if, criar_label());
    strcpy(rotulo_continue, criar_label());

    char reg1[100];
    char reg2[100];
    char reg3[100];
    strcpy(reg1, criar_registrador());
    strcpy(reg2, criar_registrador());
    strcpy(reg3, criar_registrador());
	
	if(tac_label_prox != NULL){
		
		bloco_if->tac_prev = condicional;
		conecta_bloco_ultimo_com_proximo(condicional,bloco_if);
		conecta_bloco_ultimo_com_proximo(bloco_if,tac_label_prox);		
		conecta_tacs_irmaos(tac_label_prox);
	}
	else{
		comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, busca_bloco_ultimo(condicional)->v1, rotulo_if, rotulo_continue);

		comp_list_tac_t* tac_label_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);

		comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);


		comp_list_tac_t* tac_nop = montar_tac(TAC_NOP, NULL, NULL, NULL);

		conecta_bloco_ultimo_com_proximo(condicional, tac_branch);

		conecta_tacs_irmaos(tac_branch);

		tac_label_if->tac_prev = tac_branch;

		if(bloco_if != NULL){

			bloco_if->tac_prev = tac_label_if;

			conecta_bloco_ultimo_com_proximo(bloco_if, tac_label_continue);
		}
		else{

			tac_nop->tac_prev = tac_label_if;

			tac_label_continue->tac_prev = tac_nop;

		}
		conecta_tacs_irmaos(tac_label_continue);
	}
	return condicional;

}

comp_list_tac_t *cria_tac_if_else(comp_list_tac_t *condicional, comp_list_tac_t *bloco_if, comp_list_tac_t *bloco_else){
    char rotulo_if[100], rotulo_continue[100], rotulo_else[100];
    comp_list_tac_t *tac_jump, *tac_rotulo_if, *tac_rotulo_continue, *tac_rotulo_else;
    strcpy(rotulo_if, criar_label());
    strcpy(rotulo_continue, criar_label());
    strcpy(rotulo_else, criar_label());

    char reg1[100];
    char reg2[100];
    char reg3[100];
    strcpy(reg1, criar_registrador());
    strcpy(reg2, criar_registrador());
    strcpy(reg3, criar_registrador());

    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR,busca_bloco_ultimo(condicional)->v1, rotulo_if, rotulo_else);
    comp_list_tac_t* tac_label_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);
	comp_list_tac_t* tac_nop_if = montar_tac(TAC_NOP, NULL, NULL, NULL);
	comp_list_tac_t* tac_nop_else = montar_tac(TAC_NOP, NULL, NULL, NULL);

    comp_list_tac_t* tac_jump_continue = montar_tac(TAC_JUMP_LABEL, rotulo_continue, NULL, NULL);
    comp_list_tac_t* tac_label_else = montar_tac(TAC_LABEL, rotulo_else, NULL, NULL);
    comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);

	conecta_bloco_ultimo_com_proximo(condicional, tac_branch);

	conecta_tacs_irmaos(tac_branch);

	tac_label_if->tac_prev = tac_branch;

   	if(bloco_if != NULL){
		bloco_if->tac_prev = tac_label_if;
		conecta_bloco_ultimo_com_proximo(bloco_if, tac_jump_continue);
	}
	else{
		tac_nop_if->tac_prev = tac_label_if;
		tac_jump_continue->tac_prev = tac_nop_if;
	}

	 	tac_label_else->tac_prev = tac_jump_continue;

	if(bloco_else != NULL){
		bloco_else->tac_prev = tac_label_else;
		conecta_bloco_ultimo_com_proximo(bloco_else, tac_label_continue);
	}
	else{
		tac_nop_else->tac_prev = tac_label_else;
		tac_label_continue->tac_prev = tac_nop_else;
	}

	conecta_tacs_irmaos(tac_label_continue);

    return condicional;
}

comp_list_tac_t *cria_tac_do_while(comp_list_tac_t* condicional, comp_list_tac_t* bloco_while){
    char rotulo_while[100];
    char rotulo_continue[100];
    strcpy(rotulo_while, criar_label());
    strcpy(rotulo_continue, criar_label());

    char reg1[100];
    char reg2[100];
    char reg3[100];
    strcpy(reg1, criar_registrador());
    strcpy(reg2, criar_registrador());
    strcpy(reg3, criar_registrador());


    comp_list_tac_t* tac_rotulo_while = montar_tac(TAC_LABEL, rotulo_while, NULL, NULL);
    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR,busca_bloco_ultimo(condicional)->v1, rotulo_while, rotulo_continue);
    comp_list_tac_t* tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);
	comp_list_tac_t* tac_nop = montar_tac(TAC_NOP, NULL, NULL, NULL);

	if(bloco_while != NULL){

		bloco_while->tac_prev = tac_rotulo_while;

		conecta_bloco_ultimo_com_proximo(bloco_while, condicional);

		conecta_bloco_ultimo_com_proximo(condicional, tac_branch);

	}
	else{
		tac_nop->tac_prev = tac_rotulo_while;
		condicional->tac_prev = tac_nop;
		conecta_bloco_ultimo_com_proximo(condicional, tac_branch);
	}

	tac_rotulo_continue->tac_prev = tac_branch;

	conecta_tacs_irmaos(tac_rotulo_continue);

    return tac_rotulo_while;

}

comp_list_tac_t* criar_label_antes_ultimo_bloco(comp_list_tac_t* bloco, comp_list_tac_t* label){
	comp_list_tac_t* aux = busca_bloco_ultimo(bloco);

	label->tac_prev = aux->tac_prev;
	aux->tac_prev = label;

	conecta_tacs_irmaos(aux);

	return bloco;
}

comp_list_tac_t *cria_tac_while_do(comp_list_tac_t* condicional, comp_list_tac_t* bloco_while){
    char rotulo_while[100];
    char rotulo_continue[100];
    char rotulo_jump[100];
    strcpy(rotulo_while, criar_label());
    strcpy(rotulo_continue, criar_label());
    strcpy(rotulo_jump, criar_label());

    char reg1[100];
    char reg2[100];
    char reg3[100];
    strcpy(reg1, criar_registrador());
    strcpy(reg2, criar_registrador());
    strcpy(reg3, criar_registrador());

	comp_list_tac_t* tac_rotulo_jump = montar_tac(TAC_LABEL, rotulo_jump, NULL, NULL);
	tac_rotulo_jump->tac_prev = condicional->tac_prev;
	condicional->tac_prev = tac_rotulo_jump;
	conecta_tacs_irmaos(condicional);

    comp_list_tac_t* tac_rotulo_while = montar_tac(TAC_LABEL, rotulo_while, NULL, NULL);
    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, (busca_bloco_ultimo(condicional))->v1, rotulo_while, rotulo_continue);
    comp_list_tac_t* tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);
    comp_list_tac_t* tac_jump_label = montar_tac(TAC_JUMP_LABEL, rotulo_jump, NULL, NULL);

    conecta_bloco_ultimo_com_proximo(condicional, tac_branch);
	conecta_tacs_irmaos(tac_branch);

    tac_rotulo_while->tac_prev = tac_branch;

	if(bloco_while != NULL){

		bloco_while->tac_prev = tac_rotulo_while;

		conecta_bloco_ultimo_com_proximo(bloco_while, tac_jump_label);
	}
	else{
		tac_jump_label->tac_prev = tac_rotulo_while;
	}

	tac_rotulo_continue->tac_prev = tac_jump_label;


    conecta_tacs_irmaos(tac_rotulo_continue);

    return condicional;

}

int list_tac_count(comp_list_tac_vector_t* list_args){
    int count = 0;
    comp_list_tac_vector_t* aux = list_args;
    while (aux != NULL){
        count++;
        aux = aux->next;
    }

    return count;
}

comp_list_tac_vector_t* list_tac_concat(comp_list_tac_vector_t* list_a, comp_list_tac_vector_t* list_b){
    comp_list_tac_vector_t* list_aux = list_a;
    while (list_aux != NULL){
        if (list_aux->next == NULL){
            list_a->next = list_b;
        }
        list_aux = list_aux->next;
    }

    return list_a;
}

comp_list_tac_vector_t* list_tac_create_item(comp_list_tac_t* tac){
    comp_list_tac_vector_t* new_list = malloc(sizeof(comp_list_tac_vector_t));
    new_list->tac = tac;
    new_list->next = NULL;

    return new_list;
}

comp_list_tac_t* criar_tac_expressao_logica(int operacao, comp_list_tac_t* tac1, comp_list_tac_t* tac2){
	char label0[100]; strcpy(label0, criar_label());
	char label1[100]; strcpy(label1, criar_label());
	char label2[100]; strcpy(label2, criar_label());

	if(operacao == TK_OC_AND){
			if(strcmp(tac1->v1,"0")==0){
				return tac1;
			}
			else{
				comp_list_tac_t* tac_and = montar_tac(TK_OC_AND, criar_registrador(), tac1->v1, tac2->v1);
				return tac_and;
			}
	}
	else if(operacao == TK_OC_OR){
		comp_list_tac_t* tac_brench1 = montar_tac(TAC_CBR, busca_bloco_ultimo(tac1)->v1, label0, label1);
		comp_list_tac_t* tac_brench2 = montar_tac(TAC_CBR, busca_bloco_ultimo(tac2)->v1, label0, label2);
		comp_list_tac_t* tac_label_0 =  montar_tac(TAC_LABEL, label0, NULL, NULL);
		comp_list_tac_t* tac_label_1 =  montar_tac(TAC_LABEL, label1, NULL, NULL);
		
		conecta_bloco_ultimo_com_proximo(tac1, tac_brench1);
		
		tac_label_1->tac_prev = tac_brench1;
		
		tac2->tac_prev = tac_label_1;
		
		conecta_bloco_ultimo_com_proximo(tac2, tac_brench2);
		
		conecta_bloco_ultimo_com_proximo(tac_brench2, tac_label_0);
		
		conecta_tacs_irmaos(tac_label_0);		
		
		tac_label_prox = montar_tac(TAC_LABEL, label2, NULL, NULL);
		
		return tac1;
	}
}
