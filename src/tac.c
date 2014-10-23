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
        case TAC_ATRIBUICAO:
            printf("store %s => %s\n", tac->v2, tac->v3);
            break;
        case TAC_LABEL:
            printf( "%s: \n", tac->v1);
            break;
        default:
            printf("default\n");
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

comp_list_tac_t *criar_tac_expressao(int operacao, comp_list_tac_t *tac1, comp_list_tac_t *tac2){
    comp_list_tac_t *new_tac = criar_tac();

    new_tac = montar_tac(operacao,criar_registrador(),tac1->v1,tac2->v1);
    if(tac1){
      if(tac2){
	new_tac->tac_prev = tac2;
	tac2->tac_next = tac1;
      }
      else{
	new_tac->tac_prev = tac1;
      }
    }

    conecta_tacs_irmaos(new_tac);
    return new_tac;
}

comp_list_tac_t *criar_tac_literal(int tipo, char* valor)
{
	comp_list_tac_t *new_tac = criar_tac();

	new_tac = montar_tac(tipo, valor, NULL, NULL);

	conecta_tacs_irmaos(new_tac);
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

comp_list_tac_t* criar_tac_atribuicao(char *dest, comp_list_tac_t* orig, int desloc) {
    comp_list_tac_t* tac_atr;
    char *desloc_str = (char *) malloc (100 * sizeof(char));

    sprintf(desloc_str,"%d",desloc);
    tac_atr = montar_tac(TAC_ATRIBUICAO, dest, orig->v1, desloc_str);
    tac_atr->tac_prev = orig;

    conecta_tacs_irmaos(tac_atr);
    return tac_atr;

}
