#include "tac.h"

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

comp_list_tac_t *criar_tac_literal(int tipo_literal, int tipo, char* valor, int escopo, int desloc)
{
	//fprintf(stdout, "%s\n", valor);
	comp_list_tac_t *new_tac = criar_tac();
	comp_list_tac_t *tac_desloc = criar_tac();
	
	// Cria referência para o rbss
	char *rbss = (char *) malloc (100 * sizeof(char));
    strcpy(rbss,"rbss");
	
	char *desloc_str = (char *) malloc (100 * sizeof(char));

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
			if (escopo == INTERNO) {
				sprintf(desloc_str,"%d",desloc);
				new_tac = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, valor);
				conecta_tacs_irmaos(new_tac);
			}			
			else if (escopo == EXTERNO) {
				sprintf(desloc_str,"%d",desloc);
				char* reg_desloc = criar_registrador();
				tac_desloc = montar_tac(TAC_ADD_VAL, reg_desloc, rbss, desloc_str);
				new_tac = montar_tac(TAC_LOAD, criar_registrador(), NULL, reg_desloc);
				
				new_tac->tac_prev = tac_desloc;
				conecta_tacs_irmaos(new_tac);

				return tac_desloc;
			}
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

    
    comp_list_tac_t* tac_load_val1 = montar_tac(TAC_LOAD_VAL, reg1, NULL, condicional->v2);
	
    comp_list_tac_t* tac_load_val2 = montar_tac(TAC_LOAD_VAL, reg2, NULL, condicional->v3);
    tac_load_val2->tac_prev = tac_load_val1;

    condicional->tac_prev = tac_load_val2;

    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, condicional->v1, rotulo_if, rotulo_continue);
    tac_branch->tac_prev = condicional;

    comp_list_tac_t* tac_label_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);
    tac_label_if->tac_prev = tac_branch;

    bloco_if->tac_prev = tac_label_if;

    comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);

    conecta_bloco_ultimo_com_proximo(bloco_if, tac_label_continue);

    conecta_tacs_irmaos(tac_label_continue);

    return tac_load_val1;
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

    
    comp_list_tac_t* tac_load_val1 = montar_tac(TAC_LOAD_VAL, reg1, NULL, condicional->v2);	
    comp_list_tac_t* tac_load_val2 = montar_tac(TAC_LOAD_VAL, reg2, NULL, condicional->v3);
    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, condicional->v1, rotulo_if, rotulo_else);
    comp_list_tac_t* tac_label_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);

    comp_list_tac_t* tac_jump_continue = montar_tac(TAC_JUMP_LABEL, rotulo_continue, NULL, NULL);
    comp_list_tac_t* tac_label_else = montar_tac(TAC_LABEL, rotulo_else, NULL, NULL);
    comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);


    tac_load_val2->tac_prev = tac_load_val1;
    condicional->tac_prev = tac_load_val2;
    tac_branch->tac_prev = condicional;
    tac_label_if->tac_prev = tac_branch;
    bloco_if->tac_prev = tac_label_if;

	
    conecta_bloco_ultimo_com_proximo(bloco_if, tac_jump_continue);

    tac_label_else->tac_prev = tac_jump_continue;

    bloco_else->tac_prev = tac_label_else;

    conecta_bloco_ultimo_com_proximo(bloco_else, tac_label_continue);

    conecta_tacs_irmaos(tac_label_continue);

    return tac_load_val1;
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


    comp_list_tac_t* tac_load_val1 = montar_tac(TAC_LOAD_VAL, reg1, NULL, condicional->v2);
    comp_list_tac_t* tac_load_val2 = montar_tac(TAC_LOAD_VAL, reg2, NULL, condicional->v3);
    comp_list_tac_t* tac_rotulo_while = montar_tac(TAC_LABEL, rotulo_while, NULL, NULL);
    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, condicional->v1, rotulo_while, rotulo_continue);
    comp_list_tac_t* tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);


    tac_load_val2->tac_prev = tac_load_val1;

    tac_rotulo_while->tac_prev = tac_load_val2;

    bloco_while->tac_prev = tac_rotulo_while;

    conecta_bloco_ultimo_com_proximo(bloco_while, condicional);
    
    tac_branch->tac_prev = condicional;

    tac_rotulo_continue->tac_prev = tac_branch;

    conecta_tacs_irmaos(tac_rotulo_continue);

    return tac_load_val1;

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
	
	//comp_list_tac_t* tac_aux_label = criar_label_antes_ultimo_bloco(condicional, tac_rotulo_jump);

	//print_tac(busca_bloco_ultimo(tac_aux_label));
	//print_tac(tac_rotulo_jump);
	//fprintf(stdout, "lkajkldf\n");
	
    //comp_list_tac_t* tac_load_val1 = montar_tac(TAC_LOAD_VAL, reg1, NULL, condicional->v2);
    //comp_list_tac_t* tac_load_val2 = montar_tac(TAC_LOAD_VAL, reg2, NULL, condicional->v3);
    comp_list_tac_t* tac_rotulo_while = montar_tac(TAC_LABEL, rotulo_while, NULL, NULL);
    comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, (busca_bloco_ultimo(condicional))->v1, rotulo_while, rotulo_continue);
	//comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, (busca_bloco_ultimo(tac_aux_label))->v1, rotulo_while, rotulo_continue);
    comp_list_tac_t* tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);
    comp_list_tac_t* tac_jump_label = montar_tac(TAC_JUMP_LABEL, rotulo_jump, NULL, NULL);

    //tac_load_val2->tac_prev = tac_load_val1;

    //tac_rotulo_jump->tac_prev = tac_load_val2;

    conecta_bloco_ultimo_com_proximo(condicional, tac_branch);
	conecta_tacs_irmaos(tac_branch);	
    
    //conecta_tacs_irmaos(tac_rotulo_jump);
    //print_tac(condicional);
    //getchar(); 
    
    //condicional->tac_prev = tac_rotulo_jump;

    //tac_branch->tac_prev = busca_bloco_ultimo(tac_aux_label);

    tac_rotulo_while->tac_prev = tac_branch;

    bloco_while->tac_prev = tac_rotulo_while;
    
    conecta_bloco_ultimo_com_proximo(bloco_while, tac_jump_label);

    tac_rotulo_continue->tac_prev = tac_jump_label;

    conecta_tacs_irmaos(tac_rotulo_continue);
    
    //print_tac(tac_jump_label);
    //getchar();

    return condicional;
	//return tac_aux_label;
}

