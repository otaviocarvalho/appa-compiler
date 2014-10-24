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
            printf( "%s: \n", tac->v1);
            break;
        case TAC_LOAD_VAL:
            printf( "loadI %s => %s \n", tac->v3, tac->v1);
            break;
        default:
            /*printf("default %d %s %s %s\n", tac->tipo, tac->v1, tac->v2, tac->v3);*/
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

comp_list_tac_t *criar_tac_expressao(int operacao, comp_list_tac_t *tac1, comp_list_tac_t *tac2) {
    comp_list_tac_t *new_tac = criar_tac();

    if (tac1 == NULL)
        new_tac = montar_tac(operacao, criar_registrador(), "0", tac2->v1);
    else if (tac2 == NULL)
        new_tac = montar_tac(operacao, criar_registrador(), tac1->v1, "0");
    else
        new_tac = montar_tac(operacao, criar_registrador(), tac1->v1, tac2->v1);

    if (tac1) {
        if (tac2) {
            new_tac->tac_prev = tac2;
            tac2->tac_next = tac1;
        }
        else {
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

// Preencher
comp_list_tac_t* criar_tac_chamada_funcao(char* id, comp_list_tac_t* tac_func) {
    /*fprintf(stdout, "entrou criar_tac_chamada_funcao\n");*/
    comp_list_tac_t* tac_new = criar_tac();

    return tac_new;
}

comp_list_tac_t* criar_tac_atribuicao(char *dest, comp_list_tac_t* orig, int desloc) {
    comp_list_tac_t* tac_atr;
    comp_list_tac_t* tac_load_val;
    comp_list_tac_t* tac_load_desloc;
    char *desloc_str = (char *) malloc (100 * sizeof(char));

    // Registrador para armazenar o valor a ser atribuído
    tac_load_val = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, orig->v1);
    tac_load_val->tac_prev = orig;

    // Registrador para armazenar o deslocamento
    sprintf(desloc_str,"%d",desloc);
    tac_load_desloc = montar_tac(TAC_LOAD_VAL, criar_registrador(), NULL, desloc_str);
    tac_load_desloc->tac_prev = tac_load_val;

    tac_atr = montar_tac(TAC_ATRIBUICAO, dest, tac_load_val->v1, tac_load_desloc->v1);
    tac_atr->tac_prev = tac_load_desloc;

    // Conecta do último tac até o primeiro na ordem inversa
    conecta_tacs_irmaos(tac_atr);

    // Imprime tacs na ordem da sequencia de execução
    /*print_tac(tac_load_val);*/

    return tac_load_val;
}

comp_list_tac_t *cria_tac_if(comp_list_tac_t *condicional, comp_list_tac_t *bloco_if){
    char rotulo_if[100], rotulo_continue[100];
    comp_list_tac_t *tac_jump, *tac_rotulo_if, *tac_rotulo_continue;
    strcpy(rotulo_if, criar_label());
    strcpy(rotulo_continue, criar_label());
    
    tac_jump = montar_tac(TAC_JUMP_SE, condicional->v1, rotulo_if, rotulo_continue);
    tac_jump->tac_prev = condicional;
    
    tac_rotulo_if = montar_tac(TAC_LABEL, rotulo_if, NULL, NULL);
    tac_rotulo_if->tac_prev = tac_jump;
    bloco_if->tac_prev = tac_rotulo_if;
    
    tac_rotulo_continue = montar_tac(TAC_LABEL, rotulo_continue, NULL, NULL);
    tac_rotulo_continue->tac_prev = bloco_if;

    conecta_tacs_irmaos(tac_jump);
    return tac_rotulo_continue;
}
