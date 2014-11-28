#include "tac.h"

comp_list_tac_t* tac_label_prox;
comp_list_tac_t* tac_label_inicio;
int curto_circuito = OFF;

int registrador_atual = 0;
int label_atual = 0;
FILE* arquivo;
FILE* arquivo_otimizado_ac;
FILE* arquivo_otimizado_sa;
FILE* arquivo_otimizado_pc;
FILE* arquivo_otimizado_ir;

comp_list_tac_t* gerar_tac_string(char* linha) {
    char* token;
    char* token_v1;
    char* token_v2;
    char* token_v3;
    int resultado;
    comp_list_tac_t* tac = criar_tac();

    if (strcmp(linha, "nop\n") == 0){
        tac = montar_tac(TAC_NOP, NULL, NULL, NULL);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }

    /*fprintf(stdout, "%s\n", linha);*/
    if (linha[0] == 'L'){
        token_v1 = strtok(linha, ":");
        tac = montar_tac(TAC_LABEL, token_v1, NULL, NULL);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }

    token = strtok(linha, " ");
    /*fprintf(stdout, "%s\n", token);*/
    /*getchar();*/
    // Identifica a instrução
    if (strcmp(token, "add") == 0){
        token_v3 = strtok(NULL, ",");
        /*fprintf(stdout, "%s\n", token_v3);*/
        token_v2 = strtok(NULL, "=> ");
        /*fprintf(stdout, "%s\n", token_v2);*/
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        /*fprintf(stdout, "%s\n", token_v1);*/
        tac = montar_tac('+', token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "sub") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac('-', token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "div") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac('/', token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "mult") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac('*', token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cmp_LE") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "-> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TK_OC_LE, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cmp_GE") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "-> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TK_OC_GE, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cmp_EQ") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "-> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TK_OC_EQ, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cmp_NE") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "-> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TK_OC_NE, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "and") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TK_OC_AND, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "or") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TK_OC_OR, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cmp_LT") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "-> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac('<', token_v2, token_v3, token_v1);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cmp_GT") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "-> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac('>', token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "store") == 0){
        token_v3 = strtok(NULL, " ");
        token_v1 = strtok(NULL, " ");
        token_v2 = strtok(NULL, "\n");
        tac = montar_tac(TAC_ATRIBUICAO, NULL, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "storeAI") == 0){
        token_v3 = strtok(NULL, " ");
        /*fprintf(stdout, "%s\n", token_v3);*/
        token_v2 = strtok(NULL, " ");
        token_v2 = strtok(NULL, ", ");
        /*fprintf(stdout, "%s\n", token_v2);*/
        token_v1 = strtok(NULL, "\n");
        token_v1 = token_v1 + 1;
        /*fprintf(stdout, "%s\n", token_v1);*/
        tac = montar_tac(TAC_STORE_DESLOC, token_v3, token_v2, token_v1);
        /*print_tac_item(tac, NULL);*/

        return tac;
    }
    if (strcmp(token, "loadI") == 0){
        token_v3 = strtok(NULL, " ");
        token_v1 = strtok(NULL, " ");
        token_v2 = strtok(NULL, "\n");
        tac = montar_tac(TAC_LOAD_VAL, token_v2, NULL, token_v3);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "load") == 0){
        token_v3 = strtok(NULL, " ");
        token_v1 = strtok(NULL, " ");
        token_v2 = strtok(NULL, "\n");
        tac = montar_tac(TAC_LOAD, token_v2, NULL, token_v3);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "cbr") == 0){
        token_v3 = strtok(NULL, " ");
        /*fprintf(stdout, "%s\n", token_v3);*/
        token_v2 = strtok(NULL, " ");
        token_v2 = strtok(NULL, ", ");
        /*fprintf(stdout, "%s\n", token_v2);*/
        token_v1 = strtok(NULL, "\n");
        token_v1 = token_v1 + 1;
        /*fprintf(stdout, "%s\n", token_v1);*/
        tac = montar_tac(TAC_CBR, token_v3, token_v2, token_v1);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "jump") == 0){
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TAC_JUMP, token_v1, NULL, NULL);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "jumpI") == 0){
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TAC_JUMP_LABEL, token_v1, NULL, NULL);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "addI") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TAC_ADD_VAL, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "multI") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TAC_MULT_VAL, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "lshiftI") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TAC_LSHIFT_VAL, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }
    if (strcmp(token, "rshiftI") == 0){
        token_v3 = strtok(NULL, ",");
        token_v2 = strtok(NULL, "=> ");
        token_v1 = strtok(NULL, " ");
        token_v1 = strtok(NULL, "\n");
        tac = montar_tac(TAC_RSHIFT_VAL, token_v1, token_v3, token_v2);
        /*print_tac_item(tac, NULL);*/
        return tac;
    }

    return NULL;
}

comp_list_tac_t* gerar_tacs_input(FILE* yyin) {
    size_t buffer_size = MAX_LINE_LENGTH-1;
    char* buffer = malloc(buffer_size+1);


    /*fprintf(stdout, "gerar_tacs_input()\n");*/

    comp_list_tac_t* list_tacs_aux = criar_tac();
    comp_list_tac_t* list_tacs_retorno = list_tacs_aux;
    while ((getline(&buffer, &buffer_size, yyin)) > 0) {
        if (strcmp(buffer, "\r\n") == 0)
            break;
        /*fprintf(stdout, "%s", buffer);*/

        // Cria um novo tac com base na linha lida da entrada
        comp_list_tac_t* new_tac = gerar_tac_string(buffer);

        // Conecta o tac criado com a lista de tacs
        new_tac->tac_prev = list_tacs_aux;
        conecta_tacs_irmaos(new_tac);
        list_tacs_aux = new_tac;
    }

    return list_tacs_retorno;
}

void print_tac(comp_list_tac_t* raiz){
    int file_opened = 0;
    comp_list_tac_t* aux = raiz;

    /*fprintf(stdout, "optim %d\n", global_optimization_parameter);*/
    if (raiz == NULL) {
        printf("RAIZ NULL\n");
        return;
    }

    // Optimization parameter on function call (./main -O<global_optimization_parameter> <filename>.iloc)
    switch (global_optimization_parameter) {
        case 0:
            break;

        case 1:
            /*fprintf(stdout, "case 1\n");*/
            arquivo_otimizado_ac = fopen("tac_avaliacao_constantes.i","w");

            if(arquivo_otimizado_ac == NULL){
                printf("Erro ao abrir arquivo ac");
            }
            else {
                aux = raiz;
                otimizacao_avaliacao_constantes(raiz);
                arquivo = arquivo_otimizado_ac;
                file_opened = 1;
            }
            break;

        case 2:
            /*fprintf(stdout, "case 2\n");*/
            arquivo_otimizado_sa = fopen("tac_simplificacoes_algebricas.i","w");

            if(arquivo_otimizado_sa == NULL){
                printf("Erro ao abrir arquivo ac");
            }
            else {
                aux = raiz;
                otimizacao_simplificacao_algebrica(raiz);
                arquivo = arquivo_otimizado_sa;
                file_opened = 1;
            }
            break;

        case 3:
            /*fprintf(stdout, "case 3\n");*/
            arquivo_otimizado_pc = fopen("tac_propagacao_copias.i","w");

            if(arquivo_otimizado_pc == NULL){
                printf("Erro ao abrir arquivo pc");
            }
            else {
                aux = raiz;
                otimizacao_propagacao_copia(raiz);
                arquivo = arquivo_otimizado_pc;
                file_opened = 1;
            }
            break;

        default:
            /*fprintf(stdout, "heurística padrão\n");*/
            break;
    }

    // Impressão dos TACs
    while (aux != NULL){
        /*print_tac_item(aux,NULL);*/
        print_tac_item(aux, arquivo);
        aux = aux->tac_next;
    }

    if (file_opened)
        fclose(arquivo);

    return;

	/*printf("Tac Original\n");*/
    /*arquivo = fopen("tac.i", "w");*/
	/*if(arquivo != NULL){*/
			/*[>printf("deu brete");<]*/
	/*}*/

    /*while (aux != NULL){*/
        /*print_tac_item(aux,arquivo);*/
        /*aux = aux->tac_next;*/
    /*}*/
    /*fclose(arquivo);*/
	/*printf("\n\n");*/

    //Otimização de código

	//Avaliação de constantes
	/*printf("Tac avaliação de constantes\n");*/
    /*arquivo_otimizado_ac = fopen("tac_avaliacao_constantes.i","w");*/
	/*if(arquivo_otimizado_ac == NULL){*/
		/*printf("Erro ao abrir arquivo ac");*/
	/*}*/
    /*aux = raiz;*/

	/*otimizacao_avaliacao_constantes(raiz);*/

    /*while (aux != NULL){*/
        /*print_tac_item(aux,arquivo_otimizado_ac);*/
        /*aux = aux->tac_next;*/
    /*}*/
    /*fclose(arquivo_otimizado_ac);*/
	/*printf("\n\n");*/

	//Simplificações algébricas
	/*printf("Tac simplificações algébricas\n");*/
    /*arquivo_otimizado_sa = fopen("tac_simplificacoes_algebricas.i","w");*/
	/*if(arquivo_otimizado_sa == NULL){*/
		/*printf("Erro ao abrir arquivo ac");*/
	/*}*/
    /*aux = raiz;*/

	/*otimizacao_simplificacao_algebrica(raiz);*/

    /*while (aux != NULL){*/
        /*print_tac_item(aux,arquivo_otimizado_sa);*/
        /*aux = aux->tac_next;*/
    /*}*/
    /*fclose(arquivo_otimizado_sa);*/
	/*printf("\n\n");*/


	//Propagação de cópias
	/*printf("Tac propagação de cópias\n");*/
    /*arquivo_otimizado_pc = fopen("tac_propagacao_copias.i","w");*/
	/*if(arquivo_otimizado_pc == NULL){*/
		/*printf("Erro ao abrir arquivo pc");*/
	/*}*/
    /*aux = raiz;*/

	/*otimizacao_propagacao_copia(raiz);*/

    /*while (aux != NULL){*/
        /*print_tac_item(aux,arquivo_otimizado_pc);*/
        /*aux = aux->tac_next;*/
    /*}*/
    /*fclose(arquivo_otimizado_pc);*/
	/*printf("\n\n");*/
}

void print_tac_item(comp_list_tac_t* tac, FILE* arquivo){

	switch(tac->tipo){
        case '+':
            printf("add %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"add %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '-':
            printf("sub %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"sub %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '/':
            printf("div %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"div %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '*':
            printf("mult %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"mult %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case UMINUS:
            printf("sub 0, %s => %s\n", tac->v2, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"sub 0, %s => %s\n", tac->v2, tac->v1);
            break;
        case TK_OC_LE:
            printf("cmp_LE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"cmp_LE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_GE:
            printf("cmp_GE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"cmp_GE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_EQ:
            printf("cmp_EQ %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"cmp_EQ %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_NE:
            printf("cmp_NE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"cmp_NE %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_AND:
            printf("and %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"and %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TK_OC_OR:
            printf("or %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"or %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '<':
            printf("cmp_LT %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"cmp_LT %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case '>':
            printf("cmp_GT %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"cmp_GT %s, %s -> %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TAC_ATRIBUICAO:
            printf("store %s => %s\n", tac->v2, tac->v3);
            if (arquivo != NULL)
                fprintf(arquivo,"store %s => %s\n", tac->v2, tac->v3);
            break;
        case TAC_STORE_DESLOC:
            printf("storeAI %s => %s, %s\n", tac->v1, tac->v2, tac->v3);
            if (arquivo != NULL)
                fprintf(arquivo,"storeAI %s => %s, %s\n", tac->v1, tac->v2, tac->v3);
            break;
        case TAC_LABEL:
            printf("%s: \n", tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"%s: \n", tac->v1);
            break;
        case TAC_LOAD_VAL:
            printf("loadI %s => %s \n", tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"loadI %s => %s \n", tac->v3, tac->v1);
            break;
        case TAC_LOAD:
            printf("load %s => %s \n", tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"load %s => %s \n", tac->v3, tac->v1);
            break;
        case TAC_CBR:
            printf("cbr %s -> %s, %s \n", tac->v1, tac->v2, tac->v3);
            if (arquivo != NULL)
                fprintf(arquivo,"cbr %s -> %s, %s \n", tac->v1, tac->v2, tac->v3);
            break;
		case TAC_JUMP:
            printf("jump -> %s\n", tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"jumpI -> %s\n", tac->v1);
			break;
		case TAC_JUMP_LABEL:
            printf("jumpI -> %s\n", tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"jumpI -> %s\n", tac->v1);
			break;
		case TAC_ADD_VAL:
            printf("addI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo, "addI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
        case TAC_MULT_VAL:
            printf("multI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"multI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
		case TAC_LSHIFT_VAL:
            printf("lshiftI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"rshiftI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
		case TAC_RSHIFT_VAL:
            printf("rshiftI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            if (arquivo != NULL)
                fprintf(arquivo,"rshiftI %s, %s => %s\n", tac->v2, tac->v3, tac->v1);
            break;
		case TAC_NOP:
			printf("nop\n");
            if (arquivo != NULL)
                fprintf(arquivo,"nop\n");
			break;
		default:
            break;
    }
    //fclose(arquivo);
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

comp_list_tac_t *criar_tac_literal(int tipo_literal, int tipo, int tamanho_tipo, char* valor, int escopo, int desloc, comp_list_tac_vector_t* lista_dimensoes, comp_list_tac_vector_t* lista_decl) {
    comp_list_tac_t *new_tac = criar_tac();
    comp_list_tac_t *tac_desloc = criar_tac();

    // Cria referência para o rbss
    char *escopo_str = (char *) malloc (100 * sizeof(char));
    if (escopo == EXTERNO) {
        strcpy(escopo_str,"rbss");
    }
    else {
        /*strcpy(escopo_str,"rarp");*/
        strcpy(escopo_str,"rlocal_vars_offset");
    }

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

                // Calcula deslocamento
                comp_list_tac_t* desloc_vetor = calcula_tac_lista_param_desloc(lista_dimensoes, lista_decl, desloc_str, tamanho_tipo, escopo);

                // Carrega com base no deslocamento
                new_tac = montar_tac(TAC_LOAD, criar_registrador(), NULL, busca_bloco_ultimo(desloc_vetor)->v1);
                new_tac->tac_prev = busca_bloco_ultimo(desloc_vetor);

                // Conecta do último tac até o primeiro na ordem inversa
                conecta_tacs_irmaos(new_tac);

                return desloc_vetor;
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
    char *label_func = (char *) malloc(100 * sizeof(char));
    sprintf(label_func, "L%s", id);

    /*comp_list_tac_t* tac_rotulo = montar_tac(TAC_LABEL, id, NULL, NULL);*/
    comp_list_tac_t* tac_rotulo = montar_tac(TAC_LABEL, label_func, NULL, NULL);

    if(tac_func !=NULL){
        tac_func->tac_prev = tac_rotulo;
        conecta_tacs_irmaos(tac_func);
        return tac_func;
    }

    return tac_rotulo;
}

comp_list_tac_t* criar_tac_atribuicao(char *dest, comp_list_tac_t* orig, int desloc, int escopo) {
    comp_list_tac_t* tac_atr = criar_tac();
    comp_list_tac_t* tac_load_val = criar_tac();
    comp_list_tac_t* tac_load_desloc = criar_tac();
    char *desloc_str = (char *) malloc (100 * sizeof(char));
    /*char *rbss = (char *) malloc (100 * sizeof(char));*/
    char *reg_escopo = (char *) malloc (100 * sizeof(char));

    /*strcpy(rbss, "rbss");*/
    sprintf(desloc_str,"%d",desloc);

    if (escopo == EXTERNO){
        strcpy(reg_escopo, "rbss");
    }
    else {
        /*strcpy(reg_escopo, "rarp");*/
        strcpy(reg_escopo, "rlocal_vars_offset");
    }

    // Calcula o deslocamento com base no escopo da variável
    tac_load_desloc = montar_tac(TAC_ADD_VAL, criar_registrador(), reg_escopo, desloc_str);
    conecta_bloco_ultimo_com_proximo(orig, tac_load_desloc);

    // Monta o tac da atribuição
    tac_atr = montar_tac(TAC_ATRIBUICAO, NULL, (busca_bloco_ultimo(orig))->v1, tac_load_desloc->v1);
    tac_atr->tac_prev = tac_load_desloc;

    // Conecta do último tac até o primeiro na ordem inversa
    conecta_tacs_irmaos(tac_atr);

    return orig;
}

comp_list_tac_t* cria_copia_conecta_tacs(comp_list_tac_vector_t* lista){
    if (lista == NULL)
        return NULL;

    comp_list_tac_t* retorno;
    comp_list_tac_t* tac_aux;
    comp_list_tac_t* tac_connect;
    comp_list_tac_t* tac_create_next;
    comp_list_tac_vector_t* vector_aux;
    int first = 1;
    vector_aux = lista;
    while (vector_aux != NULL){
        tac_aux = vector_aux->tac;
        while (tac_aux != NULL){
            if (first) {
                retorno = montar_tac(tac_aux->tipo, tac_aux->v1, tac_aux->v2, tac_aux->v3);
                tac_connect = retorno;
                first = 0;
            }
            else {
                tac_create_next = montar_tac(tac_aux->tipo, tac_aux->v1, tac_aux->v2, tac_aux->v3);
                tac_create_next->tac_prev = tac_connect;

                conecta_tacs_irmaos(tac_create_next);
                tac_connect = tac_create_next;
            }
            tac_aux = tac_aux->tac_next;
        }
        vector_aux = vector_aux->next;
    }

    return retorno;
}

comp_list_tac_t* calcula_tac_lista_param_desloc(comp_list_tac_vector_t* lista_parametros, comp_list_tac_vector_t* lista_decl, char* desloc, int tamanho_tipo, int escopo){
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
        /*strcpy(reg_escopo, "rarp");*/
        strcpy(reg_escopo, "rlocal_vars_offset");
    }

    comp_list_tac_t* tac_connect = NULL;
    comp_list_tac_t* retorno = NULL;
    comp_list_tac_vector_t* aux = lista_parametros;
    comp_list_tac_vector_t* aux_decl = lista_decl;
    while (aux != NULL && aux_decl != NULL) {

        comp_list_tac_t* aux_bloco_ultimo = busca_bloco_ultimo(aux->tac);

        // Verifica se é a última iteração
        if (aux->next == NULL) {
            strcpy(reg_mult_desloc_vetor, criar_registrador());

            // Multiplica valor do literal inteiro da dimensão pelo tamanho do tipo
            if (tac_connect != NULL)
                tac_soma_desloc = montar_tac(TAC_MULT_VAL, reg_mult_desloc_vetor, tac_connect->v1, tamanho_str);
            else {
                tac_soma_desloc = montar_tac(TAC_MULT_VAL, reg_mult_desloc_vetor, aux_bloco_ultimo->v1, tamanho_str);
            }

            // Soma ponteiro do registrador base com o deslocamento do início do array
            comp_list_tac_t* tac_soma_desloc_base = montar_tac(TAC_ADD_VAL, criar_registrador(), reg_escopo, desloc);

            // Soma ponteiro para o início do array com deslocamento do array calculado
            comp_list_tac_t* tac_soma_desloc_fim = montar_tac('+', criar_registrador(), tac_soma_desloc_base->v1, tac_soma_desloc->v1);

            tac_soma_desloc_base->tac_prev = tac_soma_desloc;
            tac_soma_desloc_fim->tac_prev = tac_soma_desloc_base;

            // Conecta o laço e ajusta o retorno
            if (tac_connect != NULL)
                tac_soma_desloc->tac_prev = tac_connect;
            else {
                retorno = tac_soma_desloc;
            }
            tac_connect = tac_soma_desloc;

            // Conecta tacs das outras dimensões com os criados para a última dimensão
            conecta_tacs_irmaos(tac_soma_desloc_fim);
        }
        else {
            // tac i * len[2] + j p/ (m[i,j])
            comp_list_tac_t* tac_mult_dim_array_decl = montar_tac('*', criar_registrador(), busca_bloco_ultimo(aux->tac)->v1, busca_bloco_ultimo(aux_decl->next->tac)->v1);
            comp_list_tac_t* tac_soma_dim_array_desloc = montar_tac('+', criar_registrador(), tac_mult_dim_array_decl->v1,
                                                                                                busca_bloco_ultimo(aux->next->tac)->v1);

            tac_soma_dim_array_desloc->tac_prev = tac_mult_dim_array_decl;
            conecta_tacs_irmaos(tac_soma_dim_array_desloc);

            // Conecta o laço e ajusta o retorno
            if (tac_connect != NULL) {
                tac_mult_dim_array_decl->tac_prev = tac_connect;
            }
            else {
                retorno = tac_mult_dim_array_decl;
            }
            tac_connect = tac_soma_dim_array_desloc;
        }

        aux_decl = aux_decl->next;
        aux = aux->next;
    }

    return retorno;
}

comp_list_tac_vector_t* cria_copia_tac(comp_list_tac_vector_t* vector) {
    comp_list_tac_vector_t* copia = malloc(sizeof(comp_list_tac_vector_t) * list_tac_count(vector));

    comp_list_tac_vector_t* aux = vector;
    while (aux != NULL){
        memcpy(copia, aux, sizeof(comp_list_tac_vector_t));
        aux = aux->next;
    }

    return copia;
}

comp_list_tac_t* criar_tac_atribuicao_vetor(char *dest, comp_list_tac_t* orig, comp_list_tac_vector_t* lista_parametros, comp_list_tac_vector_t* lista_decl, int desloc, int tamanho_tipo, int escopo) {
    comp_list_tac_t* tac_atr = criar_tac();
    comp_list_tac_t* tac_load_val = criar_tac();
    comp_list_tac_t* tac_load_desloc = criar_tac();
    comp_list_tac_t* desloc_vetor = criar_tac();

    char *desloc_str = (char *) malloc (100 * sizeof(char));
    char *reg_escopo = (char *) malloc (100 * sizeof(char));

    sprintf(desloc_str,"%d",desloc);
    if (escopo == EXTERNO){
        strcpy(reg_escopo, "rbss");
    }
    else {
        /*strcpy(reg_escopo, "rarp");*/
        strcpy(reg_escopo, "rlocal_vars_offset");
    }

    // Calcula o deslocamento com base no escopo da variável
    desloc_vetor = calcula_tac_lista_param_desloc(lista_parametros, lista_decl, desloc_str, tamanho_tipo, escopo);
    conecta_bloco_ultimo_com_proximo(orig, desloc_vetor);

    // Atribui com base no deslocamento
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

	if(curto_circuito == ON){
	    comp_list_tac_t* tac_nop_inicio = montar_tac(TAC_NOP, NULL, NULL, NULL);

	    condicional->tac_prev = tac_nop_inicio;

		conecta_bloco_ultimo_com_proximo(condicional,tac_label_inicio);

		bloco_if->tac_prev = tac_label_inicio;
		//conecta_bloco_ultimo_com_proximo(condicional,bloco_if);
		conecta_bloco_ultimo_com_proximo(bloco_if,tac_label_prox);
		conecta_tacs_irmaos(tac_label_prox);

		curto_circuito = OFF;

		return tac_nop_inicio;
	}
	else{
	    comp_list_tac_t* tac_nop_inicio = montar_tac(TAC_NOP, NULL, NULL, NULL);

		comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, busca_bloco_ultimo(condicional)->v1, rotulo_if, rotulo_continue);

		comp_list_tac_t* tac_label_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);

		comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);


		comp_list_tac_t* tac_nop = montar_tac(TAC_NOP, NULL, NULL, NULL);

		condicional->tac_prev = tac_nop_inicio;

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

		return tac_nop_inicio;
	}




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


    if(curto_circuito == ON){
        comp_list_tac_t* tac_nop = montar_tac(TAC_NOP, NULL, NULL, NULL);
        comp_list_tac_t* tac_jump_continue = montar_tac(TAC_JUMP_LABEL, rotulo_continue, NULL, NULL);
        comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);

        condicional->tac_prev = tac_nop;
        conecta_bloco_ultimo_com_proximo(condicional, tac_label_inicio);
        bloco_if->tac_prev = tac_label_inicio;
        conecta_bloco_ultimo_com_proximo(bloco_if,tac_jump_continue);
        tac_label_prox->tac_prev = tac_jump_continue;
        bloco_else->tac_prev = tac_label_prox;
        conecta_bloco_ultimo_com_proximo(bloco_else, tac_label_continue);

        conecta_tacs_irmaos(tac_label_continue);

        curto_circuito = OFF;

        return tac_nop;
    }

    else{
        comp_list_tac_t* tac_nop = montar_tac(TAC_NOP, NULL, NULL, NULL);
        comp_list_tac_t* tac_branch = montar_tac(TAC_CBR,busca_bloco_ultimo(condicional)->v1, rotulo_if, rotulo_else);
        comp_list_tac_t* tac_label_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);
        comp_list_tac_t* tac_nop_if = montar_tac(TAC_NOP, NULL, NULL, NULL);
        comp_list_tac_t* tac_nop_else = montar_tac(TAC_NOP, NULL, NULL, NULL);

        comp_list_tac_t* tac_jump_continue = montar_tac(TAC_JUMP_LABEL, rotulo_continue, NULL, NULL);
        comp_list_tac_t* tac_label_else = montar_tac(TAC_LABEL, rotulo_else, NULL, NULL);
        comp_list_tac_t* tac_label_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);

        condicional->tac_prev = tac_nop;

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

        return tac_nop;
    }
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

	if(curto_circuito == ON){
		comp_list_tac_t* tac_nop = montar_tac(TAC_NOP,NULL, NULL, NULL);
		tac_label_inicio->tac_prev = tac_nop;
		bloco_while->tac_prev = tac_label_inicio;
		conecta_bloco_ultimo_com_proximo(bloco_while, condicional);
		conecta_bloco_ultimo_com_proximo(condicional, tac_label_prox);
		conecta_tacs_irmaos(tac_label_prox);
		return tac_nop;

	}
	else{
		comp_list_tac_t* tac_nop_incio = montar_tac(TAC_NOP,NULL, NULL, NULL);
		comp_list_tac_t* tac_rotulo_while = montar_tac(TAC_LABEL, rotulo_while, NULL, NULL);
		comp_list_tac_t* tac_branch = montar_tac(TAC_CBR,busca_bloco_ultimo(condicional)->v1, rotulo_while, rotulo_continue);
		comp_list_tac_t* tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);
		comp_list_tac_t* tac_nop = montar_tac(TAC_NOP, NULL, NULL, NULL);

		if(bloco_while != NULL){

			tac_rotulo_while->tac_prev = tac_nop_incio;

			bloco_while->tac_prev = tac_rotulo_while;

			conecta_bloco_ultimo_com_proximo(bloco_while, condicional);

			conecta_bloco_ultimo_com_proximo(condicional, tac_branch);

		}
		else{

			tac_rotulo_while->tac_prev = tac_nop_incio;
			tac_nop->tac_prev = tac_rotulo_while;
			condicional->tac_prev = tac_nop;
			conecta_bloco_ultimo_com_proximo(condicional, tac_branch);
		}

		tac_rotulo_continue->tac_prev = tac_branch;

		conecta_tacs_irmaos(tac_rotulo_continue);

		return tac_nop_incio;;
	}

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
	if(curto_circuito == ON){
		comp_list_tac_t* tac_rotulo_jump = montar_tac(TAC_LABEL, rotulo_jump, NULL, NULL);
		comp_list_tac_t* tac_jump_label = montar_tac(TAC_JUMP_LABEL, rotulo_jump, NULL, NULL);
		comp_list_tac_t* tac_nop = montar_tac(TAC_NOP,NULL, NULL, NULL);

		tac_rotulo_jump->tac_prev = tac_nop;
		condicional->tac_prev = tac_rotulo_jump;
		conecta_bloco_ultimo_com_proximo(condicional,tac_label_inicio);
		bloco_while->tac_prev = tac_label_inicio;
		conecta_bloco_ultimo_com_proximo(bloco_while,tac_jump_label);
		tac_label_prox->tac_prev = tac_jump_label;
		conecta_tacs_irmaos(tac_label_prox);

		return tac_nop;
	}
	else{
		comp_list_tac_t* tac_nop = montar_tac(TAC_NOP,NULL, NULL, NULL);
		comp_list_tac_t* tac_rotulo_jump = montar_tac(TAC_LABEL, rotulo_jump, NULL, NULL);
		comp_list_tac_t* tac_rotulo_while = montar_tac(TAC_LABEL, rotulo_while, NULL, NULL);
		comp_list_tac_t* tac_branch = montar_tac(TAC_CBR, (busca_bloco_ultimo(condicional))->v1, rotulo_while, rotulo_continue);
		comp_list_tac_t* tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);
		comp_list_tac_t* tac_jump_label = montar_tac(TAC_JUMP_LABEL, rotulo_jump, NULL, NULL);

		tac_rotulo_jump->tac_prev = tac_nop;
		condicional->tac_prev = tac_rotulo_jump;
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

		return tac_nop;
	}
}

int calcula_dimensao_arranjo(comp_list_tac_vector_t* lista){
    comp_list_tac_vector_t* aux_lista = lista;
    int dimensao = 1;

    while(aux_lista != NULL){
      dimensao *= aux_lista->dimensao;
      aux_lista = aux_lista->next;
    }

    return dimensao;
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
    new_list->dimensao = 0;
    new_list->next = NULL;

    return new_list;
}

comp_list_tac_t* criar_tac_expressao_logica(int operacao, comp_list_tac_t* tac1, comp_list_tac_t* tac2){
	char label0[100]; strcpy(label0, criar_label());
	char label1[100]; strcpy(label1, criar_label());
	char label2[100]; strcpy(label2, criar_label());

	tac_label_prox = criar_tac();

	if(operacao == TK_OC_AND){

		comp_list_tac_t* tac_brench1 = montar_tac(TAC_CBR, busca_bloco_ultimo(tac1)->v1, label0, label2);
		comp_list_tac_t* tac_brench2 = montar_tac(TAC_CBR, busca_bloco_ultimo(tac2)->v1, label1, label2);
		comp_list_tac_t* tac_label_0 =  montar_tac(TAC_LABEL, label0, NULL, NULL);
		comp_list_tac_t* tac_label_1 =  montar_tac(TAC_LABEL, label1, NULL, NULL);

		conecta_bloco_ultimo_com_proximo(tac1, tac_brench1);

		tac_label_0->tac_prev = tac_brench1;

		tac2->tac_prev = tac_label_0;

		conecta_bloco_ultimo_com_proximo(tac2, tac_brench2);

// 		conecta_bloco_ultimo_com_proximo(tac_brench2, tac_label_1);
//
// 		conecta_tacs_irmaos(tac_label_1);
		conecta_tacs_irmaos(tac_brench2);

		tac_label_prox = montar_tac(TAC_LABEL, label2, NULL, NULL);

		curto_circuito = ON;

		tac_label_inicio = montar_tac(TAC_LABEL,label1, NULL, NULL);

		return tac1;

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

// 		conecta_bloco_ultimo_com_proximo(tac_brench2, tac_label_0);
//
// 		conecta_tacs_irmaos(tac_label_0);

		conecta_tacs_irmaos(tac_brench2);

		tac_label_prox = montar_tac(TAC_LABEL, label2, NULL, NULL);

		curto_circuito = ON;

		tac_label_inicio = montar_tac(TAC_LABEL,label0, NULL, NULL);

		return tac1;
	}
}

comp_list_tac_t* criar_tac_jump_main() {
    comp_list_tac_t* tac_jump = criar_tac();
    char *label_main = (char *) malloc(100 * sizeof(char));

    sprintf(label_main,"Lmain");
    tac_jump = montar_tac(TAC_JUMP_LABEL, label_main, NULL, NULL);

    return tac_jump;
}

comp_list_tac_t* criar_tac_fim_programa() {
    comp_list_tac_t* tac_label_end = criar_tac();
    char *label_end = (char *) malloc(100 * sizeof(char));

    sprintf(label_end,"Lend");
    tac_label_end = montar_tac(TAC_LABEL, label_end, NULL, NULL);

    return tac_label_end;
}

comp_list_tac_t* criar_tac_zera_sp() {
    comp_list_tac_t* tac_zera_sp = criar_tac();
    char *rsp = (char *) malloc(100 * sizeof(char));
    char *zero = (char *) malloc(100 * sizeof(char));

    sprintf(rsp, "rsp");
    sprintf(zero, "0");
    tac_zera_sp = montar_tac(TAC_LOAD_VAL, rsp, NULL, zero);

    return tac_zera_sp;
}

comp_list_tac_t* criar_tac_registro_ativacao(int linha_escopo_estatico) {
    char *rarp = (char *) malloc(100 * sizeof(char));
    char *rsp = (char *) malloc(100 * sizeof(char));
    sprintf(rarp, "rarp");
    sprintf(rsp, "rsp");

    // Atualiza fp pro antigo sp
    // fp = rarp = rsp
    comp_list_tac_t* tac_atualiza_fp = criar_tac();
    tac_atualiza_fp = montar_tac(TAC_LOAD, rarp, NULL, rsp);

    // Insere link estático
    // ve = rarp+2
    char *desloc_linha = (char *) malloc(100 * sizeof(char));
    sprintf(desloc_linha, "%d", linha_escopo_estatico);
    comp_list_tac_t* tac_load_desloc_linha = criar_tac();
    tac_load_desloc_linha = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, desloc_linha);

    char *desloc_link_pos = (char *) malloc(100 * sizeof(char));
    sprintf(desloc_link_pos, "2");
    comp_list_tac_t* tac_link_estatico = criar_tac();
    tac_link_estatico = montar_tac(TAC_STORE_DESLOC, tac_load_desloc_linha->v1, rarp, desloc_link_pos);

    // Atualiza sp pro novo topo
    // sp = rsp = rarp+3
    char *desloc_topo_sp = (char *) malloc(100 * sizeof(char));
    sprintf(desloc_topo_sp, "3");
    comp_list_tac_t* tac_desloc_topo_sp = criar_tac();
    tac_desloc_topo_sp = montar_tac(TAC_LOAD_VAL, rsp, NULL, desloc_topo_sp);

    char *label_reg_local_vars = (char *) malloc(100 * sizeof(char));
    char *label_reg_local_vars_desloc = (char *) malloc(100 * sizeof(char));
    // Registrador para o início das variáveis locais
    sprintf(label_reg_local_vars, "rlocal_vars_offset");
    /*sprintf(label_reg_local_vars_desloc, "3");*/
    comp_list_tac_t* tac_local_vars = criar_tac();
    /*tac_local_vars = montar_tac(TAC_LOAD_VAL, label_reg_local_vars, NULL, label_reg_local_vars_desloc);*/
    tac_local_vars = montar_tac(TAC_LOAD, label_reg_local_vars, NULL, rsp);

    // Juntar tacs
    /*tac_atualiza_fp->tac_prev = tac_local_vars;*/
    /*conecta_tacs_irmaos(tac_atualiza_fp);*/

    tac_load_desloc_linha->tac_prev = tac_atualiza_fp;
    tac_link_estatico->tac_prev = tac_load_desloc_linha;
    tac_desloc_topo_sp->tac_prev = tac_link_estatico;
    tac_local_vars->tac_prev = tac_desloc_topo_sp;

    /*conecta_tacs_irmaos(tac_link_estatico);*/
    /*conecta_tacs_irmaos(tac_desloc_topo_sp);*/
    conecta_tacs_irmaos(tac_local_vars);

    /*print_tac(tac_local_vars);*/
    /*fprintf(stdout, "aslkfdjkl\n");*/

    return tac_atualiza_fp;
    /*return tac_local_vars;*/
}

comp_list_tac_t* criar_tac_destroi_registro_ativacao(char* nome_funcao) {
    comp_list_tac_t* tac_retorno = criar_tac();
    char *label_main = (char *) malloc(100 * sizeof(char));

    // Retorna para o fim do código se terminou a main
    if (strcmp(nome_funcao, "main") == 0){
        char *label_end = (char *) malloc(100 * sizeof(char));

        sprintf(label_end,"Lend");
        tac_retorno = montar_tac(TAC_JUMP_LABEL, label_end, NULL, NULL);
    }
    // Retorna para a linha posterior a chamada da função
    else {
        // load rarp => rX
        // jump -> rX
        comp_list_tac_t* tac_load = criar_tac();
        comp_list_tac_t* tac_jump = criar_tac();
        char *label_rarp = (char *) malloc(100 * sizeof(char));
        sprintf(label_rarp,"rarp");

        tac_load = montar_tac(TAC_LOAD, criar_registrador(), NULL, label_rarp);
        tac_jump = montar_tac(TAC_JUMP, tac_load->v1, NULL, NULL);

        tac_jump->tac_prev = tac_load;
        conecta_tacs_irmaos(tac_jump);

        tac_retorno = tac_load;
    }

    return tac_retorno;
}

comp_list_tac_t* criar_tac_chamada_funcao(char* id, comp_list_tac_t* tac_func, int linha_retorno, int linha_vinc_dinamico) {
    char *rsp = (char *) malloc (100 * sizeof(char));
    char *val = (char *) malloc (100 * sizeof(char));
    char *str_link_vinc_dinamico = (char *) malloc (100 * sizeof(char));
    char *desloc_vinc_dinamico = (char *) malloc (100 * sizeof(char));

    // Atualiza link de retorno no próximo escopo, a partir do sp do escopo atual (fp[0] do escopo a ser criado)
    sprintf(rsp, "rsp");
    sprintf(val, "rL%d", linha_retorno);
    comp_list_tac_t* tac_end_retorno = criar_tac();
    tac_end_retorno = montar_tac(TAC_LOAD_VAL, rsp, NULL, val);

    // Insere link dinâmico
    // ve = rsp+1
    sprintf(str_link_vinc_dinamico, "%d", linha_vinc_dinamico);
    sprintf(desloc_vinc_dinamico, "1");
    comp_list_tac_t* tac_vinc_dinamico = criar_tac();
    tac_vinc_dinamico = montar_tac(TAC_STORE_DESLOC, str_link_vinc_dinamico, rsp, desloc_vinc_dinamico);

    // Passa o controle para a função chamada
    char *label_funcao = (char *) malloc (100 * sizeof(char));
    sprintf(label_funcao, "L%s", id);
    comp_list_tac_t* tac_jump_funcao = criar_tac();
    tac_jump_funcao = montar_tac(TAC_JUMP_LABEL, label_funcao, NULL, NULL);

    // Conecta tacs
    tac_vinc_dinamico->tac_prev = tac_end_retorno;
    tac_jump_funcao->tac_prev = tac_vinc_dinamico;

    /*conecta_tacs_irmaos(tac_vinc_dinamico);*/
    conecta_tacs_irmaos(tac_jump_funcao);

    return tac_end_retorno;
}

void otimizacao_avaliacao_constantes(comp_list_tac_t* lista){
	comp_list_tac_t* ptaux;
	ptaux = lista;
	while(ptaux !=NULL){

		if(ptaux->tipo == '+'){
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")==0 && strcmp(ptaux->tac_prev->v3,"0")!=0){
				ptaux->tac_prev->tac_prev->tac_prev->tac_next = ptaux->tac_prev;
				ptaux->tac_prev->tac_prev = ptaux->tac_prev->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->v1);
				ptaux->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev;
			}
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")!=0 && strcmp(ptaux->tac_prev->v3,"0")==0){
				ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->tac_prev->v1);
			}
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")==0 && strcmp(ptaux->tac_prev->v3,"0")==0){
				ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->tac_prev->v1);
			}
		}

		if(ptaux->tipo == '*'){
			//multiplicaçao por um
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"1")==0 && strcmp(ptaux->tac_prev->v3,"1")!=0){
				ptaux->tac_prev->tac_prev->tac_prev->tac_next = ptaux->tac_prev;
				ptaux->tac_prev->tac_prev = ptaux->tac_prev->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->v1);
				ptaux->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev;
			}
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"1")!=0 && strcmp(ptaux->tac_prev->v3,"1")==0){
				ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->tac_prev->v1);
			}
			//multiplicação por zero
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")==0 || strcmp(ptaux->tac_prev->v3,"0")==0){
				char reg[100];
				strcpy(reg, criar_registrador());
				comp_list_tac_t* tac = criar_tac();
				tac = montar_tac(TAC_LOAD_VAL, reg, NULL, "0");
				ptaux->tac_prev->tac_prev->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev->tac_prev->tac_prev;
				tac->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = tac;
				strcpy(ptaux->tac_next->tac_next->v2, reg);
			}
		}

		if(ptaux->tipo == '/'){
			//divisão por um
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")!=0 && strcmp(ptaux->tac_prev->v3,"1")==0){
				ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->tac_prev->v1);
			}
			//zero dividido por algo
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")==0 && strcmp(ptaux->tac_prev->v3,"0")!=0){
				ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->tac_prev->v1);
			}
		}

		if(ptaux->tipo == '-'){
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"0")!=0 && strcmp(ptaux->tac_prev->v3,"0")==0){
				ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = ptaux->tac_prev->tac_prev;
				strcpy(ptaux->tac_next->tac_next->v2, ptaux->tac_prev->tac_prev->v1);
			}
			if(strcmp(ptaux->tac_prev->tac_prev->v3, ptaux->tac_prev->v3)==0){
				char reg[100];
				strcpy(reg, criar_registrador());
				comp_list_tac_t* tac = criar_tac();
				tac = montar_tac(TAC_LOAD_VAL, reg, NULL, "0");
				ptaux->tac_prev->tac_prev->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev->tac_prev->tac_prev;
				tac->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = tac;
				strcpy(ptaux->tac_next->tac_next->v2, reg);
			}
		}

		ptaux = ptaux->tac_next;
	}
}

void otimizacao_simplificacao_algebrica(comp_list_tac_t* lista){
	comp_list_tac_t* ptaux;
	ptaux = lista;
	while(ptaux !=NULL){
		//Multiplicação por 2
		if(ptaux->tipo == '*'){
			if(strcmp(ptaux->tac_prev->tac_prev->v3,"2")==0){
				char reg[100];
				comp_list_tac_t* tac;
				strcpy(reg, criar_registrador());
				tac = criar_tac();
				tac = montar_tac(TAC_LSHIFT_VAL, ptaux->v1, ptaux->tac_prev->v1, "1");
				ptaux->tac_prev->tac_prev->tac_prev->tac_next = ptaux->tac_prev;
				ptaux->tac_prev->tac_prev = ptaux->tac_prev->tac_prev->tac_prev;
				ptaux->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev;
				tac->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = tac;
			}

			if(strcmp(ptaux->tac_prev->v3,"2")==0){
				char reg[100];
				comp_list_tac_t* tac;
				strcpy(reg, criar_registrador());
				tac = criar_tac();
				tac = montar_tac(TAC_LSHIFT_VAL, ptaux->v1, ptaux->tac_prev->tac_prev->v1, "1");

				ptaux->tac_prev->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev->tac_prev;
				ptaux->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev;
				tac->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = tac;
			}
		}
		if(ptaux->tipo == '/'){

			if(strcmp(ptaux->tac_prev->v3,"2")==0){
				char reg[100];
				comp_list_tac_t* tac;
				strcpy(reg, criar_registrador());
				tac = criar_tac();
				tac = montar_tac(TAC_RSHIFT_VAL, ptaux->v1, ptaux->tac_prev->tac_prev->v1, "1");

				ptaux->tac_prev->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev->tac_prev;
				ptaux->tac_prev->tac_next = tac;
				tac->tac_prev = ptaux->tac_prev;
				tac->tac_next = ptaux->tac_next;
				ptaux->tac_next->tac_prev = tac;
			}
		}
		ptaux = ptaux->tac_next;
	}
}

void otimizacao_propagacao_copia(comp_list_tac_t* lista){
	comp_list_tac_t* ptaux;
	comp_list_tac_t* ptaux2;
	ptaux = lista;
	int usado = 0;
	char* deslocamento;
	while(ptaux != NULL){
		if(ptaux->tipo == TAC_ADD_VAL && strcmp(ptaux->v2,"rbss") == 0){
			strcpy(deslocamento,ptaux->v3);

			ptaux2 = ptaux->tac_next;
			while(ptaux2!=NULL){
				if(ptaux2->tipo == TAC_ADD_VAL && (strcmp(ptaux2->v2,"rbss") == 0) && (strcmp(deslocamento,ptaux2->v3) == 0)){
					usado = 1;
				}
				ptaux2 = ptaux2->tac_next;
			}

			//otimiza
			if(usado == 0){
				if(ptaux->tac_next->tac_next!=NULL){
					ptaux->tac_prev->tac_prev->tac_next = ptaux->tac_next->tac_next;
					ptaux->tac_next->tac_next->tac_prev = ptaux->tac_prev->tac_prev;

				}
			}
		}
		usado = 0;
	ptaux = ptaux->tac_next;
	}
}

void otimizacao_instrucoes_redundantes(comp_list_tac_t* lista){
	comp_list_tac_t* ptaux;
	comp_list_tac_t* ptaux2;
	comp_list_tac_t* ptaux3;
	ptaux = lista;
	int otimiza = 0;
	char* deslocamento;

	while(ptaux != NULL){
		if(ptaux->tipo == TAC_ADD_VAL && strcmp(ptaux->v2,"rbss" ) == 0  && ptaux->tac_next->tipo == TAC_ATRIBUICAO){
			strcpy(deslocamento,ptaux->v3);

			ptaux2 = ptaux->tac_next;
			while(ptaux2 != NULL){
				if(ptaux2->tipo == TAC_ADD_VAL && (strcmp(ptaux2->v2,"rbss") == 0) && ptaux2->tac_next->tipo == TAC_ATRIBUICAO){
					if((strcmp(deslocamento,ptaux2->v3) == 0)){
						//otimiza o barato
						otimiza = 1;
						break;
					}
					else{
						//não otimiza o barato
						otimiza = 0;
						break;
					}
				}
				ptaux2 = ptaux2->tac_next;
			}

			//otimiza
			if(otimiza == 1){
				ptaux3 = ptaux;
				while(ptaux3->tipo != TAC_LABEL && ptaux3->tipo != TAC_NOP && ptaux3->tipo != TAC_ATRIBUICAO){
// 					fprintf(stdout, "%s\n",ptaux3->v1);
// 					getchar();
					ptaux3 = ptaux3->tac_prev;
				}

					fprintf(stdout, "%s\n",ptaux3->v1);
					fprintf(stdout, "%s\n",ptaux3->tac_next->v1);
					fprintf(stdout, "%s\n",ptaux->v1);
					fprintf(stdout, "%s\n",ptaux->tac_next->tac_next->v1);
					fprintf(stdout, "%s\n",ptaux->tac_next->tac_next->v1);
					getchar();


				ptaux3->tac_next = ptaux->tac_next->tac_next;
				ptaux->tac_next->tac_next->tac_prev = ptaux3;
				return;
			}
		}

		ptaux = ptaux->tac_next;
	}
}
