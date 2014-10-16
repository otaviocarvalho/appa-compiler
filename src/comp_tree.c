#include "comp_tree.h"

int count_nodes = 0;

comp_tree_t* create_node(int type, char* lex, comp_tree_t* node, comp_dict_item_t* hash){
    comp_tree_t* aux;
    comp_tree_t* new_node;

    // Create new node
    new_node = create_empty_node();
    new_node->type = type;
    if(hash != NULL){
        new_node->hash = hash;
    }
    new_node->id = count_nodes;
    count_nodes++;
    /*gv_declare(type, new_node, lex);*/

    // Connect nodes
    new_node->lex = lex;
    new_node->children = node;

    aux = node;
    while (aux != NULL){
        /*gv_connect(new_node, aux);*/
        aux = aux->next_brother;
    }

    return new_node;
}

void connect_nodes(comp_tree_t* node_a, comp_tree_t* node_b){
    comp_tree_t* aux;

    if (node_a != NULL && node_b != NULL){
        // Lógica de conexão da árvore
        if (node_a->children == NULL){
            node_a->children = node_b;
        }
        else {
            // Busca o último nodo do próximo nível
            aux = node_a->children;
            while (aux->next_brother != NULL){
                aux = aux->next_brother;
            }

            aux->next_brother = node_b;
        }

        // Conexão dos nodos da imagem gerada
        /*gv_connect(node_a, node_b);*/
        /*print_syntax_tree(node_a);*/
    }
}

void free_syntax_tree(comp_tree_t* syntax_tree){

//  ################# Primeira versão do Edson
//  ################# Parece não estar funcionando devido a atribuição invertida
//     if(syntax_tree != NULL)
//     {
// 	comp_tree_t* aux;// = (comp_tree_t*) malloc(sizeof(comp_tree_t);
// 	aux = syntax_tree;
// 	if(aux->next_brother != NULL)
// 	{
// 	    free_syntax_tree(aux->next_brother);
// 	}
//
// 	if(aux->children != NULL)
// 	{
// 	    free_syntax_tree(aux->children);
// 	}
// 	  free(aux->lex);
// 	  free(aux->hash);
// 	  free(aux);
// 	  return;
//     }


  //	Segunda versão tenando ir em profundidade primeiro
  //	Também não foi
  /*if(syntax_tree != NULL)*/
  /*{*/
      /*comp_tree_t* aux = syntax_tree;*/
      /*if(aux->children != NULL)*/
      /*{*/
	  /*free_syntax_tree(aux->children);*/
      /*}*/

      /*if(aux->next_brother != NULL)*/
      /*{*/
	  /*free_syntax_tree(aux->next_brother);*/
      /*}*/

      /*free(aux->lex);*/
      /*free(aux->hash);*/
      /*free(aux);*/
      /*return;      */


  /*}*/

//   if(syntax_tree != NULL)
//   {
//       comp_tree_t* aux = syntax_tree;
//       if(aux->children != NULL)
//       {
// 	  free_syntax_tree(aux->children);
//       }
//
//       if(aux->next_brother != NULL)
//       {
// 	  free_syntax_tree(aux->next_brother);
//       }
//
//
//       free(aux->lex);
//       free(aux->hash);
//       free(aux);
//       return;
//
//   }
//   ################################### Versão do Otávio
//     comp_tree_t* aux_brother;
//     comp_tree_t* aux_children;
//     comp_tree_t* last_children;
//
//     // Raíz vazia
//     if (syntax_tree == NULL)
//         return;
//
//     aux_children = syntax_tree;
//     while (aux_children != NULL){
//         /*fprintf(stdout, "free_syntax_tree %s %d\n", aux_children->lex, aux_children->type);*/
//
//         if (aux_children->children == NULL && aux_children != NULL){
//             if (aux_children == syntax_tree){
//                 free(aux_children->lex);
//                 free(aux_children->hash);
//                 free(aux_children);
//                 return;
//             }
//             else {
//                 free(aux_children->lex);
//                 free(aux_children->hash);
//                 free(aux_children);
//                 last_children->children = NULL;
//                 aux_children = syntax_tree;
//             }
//         }
//         else {
//             last_children = aux_children;
//             aux_children = aux_children->children;
//         }
//     }
}

void print_syntax_tree(comp_tree_t* syntax_tree){
    comp_tree_t* aux_brother;
    comp_tree_t* aux_children;

    aux_children = syntax_tree;
    while (aux_children != NULL){
        fprintf(stderr, "aux children %s type-iks %d\n", aux_children->lex, aux_children->type);

        if (aux_children->next_brother != NULL){
            print_syntax_tree(aux_children->next_brother);
        }

        fprintf(stdout, "print children\n");
        aux_children = aux_children->children;
    }
}

comp_tree_t* create_empty_node(){
    comp_tree_t* new_node = (comp_tree_t *) malloc(sizeof(comp_tree_t));
    new_node->type = -1;
    new_node->id = -1;
    new_node->lex = NULL;
    new_node->hash = NULL;
    new_node->list_args = NULL;
    new_node->children = NULL;
    new_node->next_brother = NULL;

    return new_node;
}

// Verifica se o filho de output é um literal
void verifica_output(comp_tree_t* node){
    if (node->children != NULL && node->children->hash != NULL) {
        if (node->children->hash->type != IKS_SIMBOLO_LITERAL_STRING){
            exit(IKS_ERROR_WRONG_PAR_OUTPUT);
        }
    }
}

// Verifica se o filho de input é um identificador
void verifica_input(comp_tree_t* node){
    if (node->children != NULL && node->children->hash != NULL) {
        if (node->children->hash->type != IKS_SIMBOLO_IDENTIFICADOR){
            exit(IKS_ERROR_WRONG_PAR_INPUT);
        }
    }
}

comp_tree_t* find_node_by_label(comp_tree_t* node, char* label){
    comp_tree_t* aux_brother;
    comp_tree_t* aux_children;
    comp_tree_t* aux_search = NULL;

    aux_children = node;
    while (aux_children != NULL && aux_search == NULL){
        if (aux_children->lex == label){
            return aux_children;
        }

        if (aux_children->next_brother != NULL)
            aux_search = find_node_by_label(aux_children->next_brother, label);

        aux_children = aux_children->children;
    }

    return aux_search;
}

comp_tree_t* find_node_by_label_and_type(comp_tree_t* node, char* label, int type){
    comp_tree_t* aux_brother;
    comp_tree_t* aux_children;
    comp_tree_t* aux_search = NULL;

    aux_children = node;
    while (aux_children != NULL && aux_search == NULL){
        if (aux_children->lex == label && aux_children->type == type){
            return aux_children;
        }

        if (aux_children->next_brother != NULL)
            aux_search = find_node_by_label(aux_children->next_brother, label);

        aux_children = aux_children->children;
    }

    return aux_search;
}

comp_tree_t* find_node_by_label_and_operator(comp_tree_t* node, char* label, int operador){
    comp_tree_t* aux_brother;
    comp_tree_t* aux_children;
    comp_tree_t* aux_search = NULL;

    aux_children = node;
    while (aux_children != NULL && aux_search == NULL){
        if (aux_children->lex == label && aux_children->hash && aux_children->hash->operador == operador){
            return aux_children;
        }

        if (aux_children->next_brother != NULL)
            aux_search = find_node_by_label(aux_children->next_brother, label);

        aux_children = aux_children->children;
    }

    return aux_search;
}

comp_tree_t* find_node_by_type(comp_tree_t* node, int type){
    int flag = -1;
    comp_tree_t* aux_brother;
    comp_tree_t* aux_children;
    comp_tree_t* aux_search = NULL;

    aux_children = node;
    while (aux_children != NULL && aux_search == NULL){
        if (aux_children->type == type){
            return aux_children;
        }

        if (aux_children->next_brother != NULL)
            aux_search = find_node_by_type(aux_children->next_brother, type);

        aux_children = aux_children->children;
    }

    return aux_search;
}

// Verifica se o return corresponde com o tipo da função
void verifica_return(comp_tree_t* node, char* label_function, int type_var_function){
    int found = -1;
    comp_tree_t* return_node = NULL;
    comp_tree_t* node_aux = node;
    comp_tree_t* node_aux_label = NULL;
    comp_tree_t* node_aux_type = NULL;

    // Encontra a função na árvore
    node_aux_label = find_node_by_label(node_aux, label_function);
    // Encontra o return na árvore
    node_aux_type = find_node_by_type(node_aux_label, IKS_AST_RETURN);

    if (node_aux_type != NULL && node_aux_type->children != NULL){
        if (node_aux_type->children->hash->type_var != type_var_function) {
            exit(IKS_ERROR_WRONG_PAR_RETURN);
        }
    }
}

// Verifica o número e o tipo dos argumentos
void verifica_argumentos(comp_tree_t* node, char* label_function, comp_list_t* list_args){
    int i;
    int count_call = 0;
    int count_decl = 0;
    /*comp_tree_t* node_aux = node;*/
    comp_list_t* aux_call = NULL;
    comp_list_t* aux_decl = NULL;

    // Conta o número de parâmetros na declaração da função
    if (list_args != NULL){
        count_call = list_count(list_args);
    }

    // Encontra a chamada da função na stack
    comp_dict_item_t* item_decl = encontra_item_operador(label_function, DECLARACAO_FUNCAO);

    if (item_decl != NULL){
        count_decl = item_decl->count_args;

        if (count_decl == count_call){
            // Verificar tipos dos argumentos
            aux_call = list_args;
            aux_decl = item_decl->list_args;
            for (i = 0; i < count_decl; i++){
                if (aux_call != NULL && aux_decl != NULL){
                    if (aux_call->item->type_var != aux_decl->item->type_var){
                        exit(IKS_ERROR_WRONG_TYPE_ARGS);
                    }
                    aux_call = aux_call->next;
                    aux_decl = aux_decl->next;
                }
            }
        }
        else if (count_decl > count_call){
            exit(IKS_ERROR_MISSING_ARGS);
        }
        else {
            exit(IKS_ERROR_EXCESS_ARGS);
        }
    }
}


void verifica_funcao(comp_tree_t* node, char* label_function){
    comp_dict_item_t* item_aux = NULL;

    if (node == NULL)
        return;

    item_aux = encontra_item_operador(label_function, DECLARACAO_FUNCAO);
    fprintf(stdout, "verifica funcao %s %d\n", item_aux->key, item_aux->type_var);

    if (item_aux != NULL){
        node->hash->type_var = item_aux->type_var;
    }

}


void verifica_coersao_arvore(comp_tree_t* node_pai, comp_tree_t* node_filho_esq, comp_tree_t* node_filho_dir){

  int tipo1;
  int tipo2;

    fprintf(stdout, "node pai lex %s type %d\n", node_pai->lex, node_pai->type);
    fprintf(stdout, "node filho esq lex %s type %d\n", node_filho_esq->lex, node_filho_esq->type);
    fprintf(stdout, "node filho dir lex %s type %d\n", node_filho_dir->lex, node_filho_dir->type);
    getchar();

  if(node_filho_esq->hash != NULL){
      switch(node_filho_esq->hash->operador){
        case USO_LITERAL:{
          tipo1 = encontra_tipo(node_filho_esq->hash->key,USO_LITERAL);
          break;
        }
        case USO_FUNCAO:{
          tipo1 = encontra_tipo(node_filho_esq->hash->key,USO_FUNCAO);
          break;
        }
        case USO_VARIAVEL:{
          tipo1 = encontra_tipo(node_filho_esq->hash->key,USO_VARIAVEL);
          break;
        }
        case USO_VETOR_INDEXADO:{
          tipo1 = encontra_tipo(node_filho_esq->hash->key,USO_VETOR_INDEXADO);
          break;
        }
      }
  }
  else{
    tipo1 = node_pai->tipo_coersao;
  }

  if(node_filho_dir->hash != NULL){

    switch(node_filho_dir->hash->operador){
        case USO_LITERAL:{
          tipo2 = encontra_tipo(node_filho_dir->hash->key,USO_LITERAL);
          break;
        }
        case USO_FUNCAO:{
          tipo2 = encontra_tipo(node_filho_dir->hash->key,USO_FUNCAO);
          break;
        }
        case USO_VARIAVEL:{
          tipo2 = encontra_tipo(node_filho_dir->hash->key,USO_VARIAVEL);
          break;
        }
        case USO_VETOR_INDEXADO:{
          tipo2 = encontra_tipo(node_filho_dir->hash->key,USO_VETOR_INDEXADO);
          break;
        }
    }

  }

  else{
    tipo2 = node_filho_dir->tipo_coersao;
  }


  if(tipo1 == tipo2){
    node_pai->tipo_coersao = tipo1;
  }

  if((tipo1 == IKS_FLOAT && tipo2 == IKS_INT) ||
    (tipo1 == IKS_INT && tipo2 == IKS_FLOAT)){
    node_pai->tipo_coersao = IKS_FLOAT;
  }
  if((tipo1 == IKS_BOOL && tipo2 == IKS_INT) ||
    (tipo1 == IKS_INT && tipo2 == IKS_BOOL)) {
    node_pai->tipo_coersao = IKS_INT;
  }
  if((tipo1 == IKS_BOOL && tipo2 == IKS_FLOAT) ||
    (tipo1 == IKS_FLOAT && tipo2 == IKS_BOOL)) {
    node_pai->tipo_coersao = IKS_FLOAT;
  }
  if(((tipo1 == IKS_FLOAT || tipo1 == IKS_INT || tipo1 == IKS_BOOL) && (tipo2 == IKS_STRING))||
    (((tipo2 == IKS_FLOAT || tipo2 == IKS_INT || tipo2 == IKS_BOOL) && (tipo1 == IKS_STRING)))){
     exit(IKS_ERROR_STRING_TO_X);
    }
  if(((tipo1 == IKS_FLOAT || tipo1 == IKS_INT || tipo1 == IKS_BOOL) && (tipo2 == IKS_CHAR))||
    (((tipo2 == IKS_FLOAT || tipo2 == IKS_INT || tipo2 == IKS_BOOL) && (tipo1 == IKS_CHAR)))){
     exit(IKS_ERROR_CHAR_TO_X);
    }
}

void verifica_atribuicao(comp_tree_t* node,int tipo){



    int operador;
    if(node->hash != NULL){
        operador = encontra_operador(node->hash->key);
        if(operador == USO_VARIAVEL){

            int tipo_variavel = encontra_tipo(node->hash->key,DECLARACAO_VARIAVEL);
            if(tipo != tipo_variavel){//variáveis de tipos diferentes
                if(tipo == IKS_INT){//Variável de tipo INT lado esquerdo
                    if(tipo_variavel == IKS_STRING){
                        exit(IKS_ERROR_STRING_TO_X);
                    }
                    if(tipo_variavel  == IKS_CHAR){
                        exit(IKS_ERROR_CHAR_TO_X);
                    }
                }

                if(tipo == IKS_CHAR){//Variável tipo CHAR lado esquerdo
                    if(tipo_variavel == IKS_STRING){
                        exit(IKS_ERROR_STRING_TO_X);
                    }
                    if(tipo_variavel == IKS_FLOAT){
                        exit(IKS_ERROR_WRONG_TYPE);
                    }
                }
                if(tipo == IKS_STRING){//Variável tipo STRING lado esquerdo
                    if(tipo_variavel  == IKS_CHAR){
                        exit(IKS_ERROR_CHAR_TO_X);
                    }
                }
            }
        }

        if(operador == USO_FUNCAO){
            int tipo_funcao = encontra_tipo(node->hash->key,DECLARACAO_FUNCAO);
            fprintf(stdout, "verifica tipo declaracao funcao %d %d\n", tipo_funcao, tipo);
            if(tipo != tipo_funcao){ //Retorno de função com tipo diferente
                if(tipo == IKS_INT){ //Variável de tipo INT lado esquerdo
                    if(tipo_funcao == IKS_STRING){
                        exit(IKS_ERROR_STRING_TO_X);
                    }
                    if(tipo_funcao  == IKS_CHAR){
                        exit(IKS_ERROR_CHAR_TO_X);
                    }
                }

                if(tipo == IKS_CHAR){//Variável tipo CHAR lado esquerdo
                    if(tipo_funcao == IKS_STRING){
                        exit(IKS_ERROR_STRING_TO_X);
                    }
                }

                /*if(tipo == IKS_STRING){//Variável tipo STRING lado esquerdo*/
                    /*if(tipo_funcao  == IKS_INT){*/
                        /*exit(IKS_ERROR_WRONG_TYPE);*/
                    /*}*/
                /*}*/

                exit(IKS_ERROR_WRONG_TYPE);
            }
        }

        if(operador == USO_VETOR_INDEXADO){
        }

        if(operador == USO_LITERAL){
            int tipo_literal = encontra_tipo(node->hash->key,USO_LITERAL);
            if(tipo != tipo_literal){ //Retorno de função com tipo diferente
                if(tipo == IKS_CHAR){//Variável tipo CHAR lado esquerdo
                    if(tipo_literal == IKS_FLOAT){
                        exit(IKS_ERROR_WRONG_TYPE);
                    }
                }
                if(tipo == IKS_STRING){
                    if(tipo_literal == IKS_FLOAT){
                        exit(IKS_ERROR_WRONG_TYPE);
                    }
                }
            }
        }
    }
    else{
        fprintf(stdout, "Vamos ver o sinal da expressão => %d\n", node->tipo_coersao);
        getchar();
    }
}

void verifica_tipo_indexador(comp_tree_t* node){
    int operador = encontra_operador(node->hash->key);

    if(operador == USO_LITERAL){
        int tipo_literal = encontra_tipo(node->hash->key,USO_LITERAL);
        if(tipo_literal != IKS_INT){
            if(tipo_literal == IKS_STRING){
                exit(IKS_ERROR_STRING_TO_X);
            }
        }

        if(tipo_literal == IKS_CHAR){
            exit(IKS_ERROR_CHAR_TO_X);
        }
    }

}

comp_dict_item_t* encontra_item_operador(char* key, int operador){
    int hash = hash_function(key);

    comp_stack_dict_t* ptaux = stack_scope;

    while(ptaux != NULL){
       if ((ptaux->dict->entries[hash] == NULL)){
            ptaux = ptaux->next;
            continue;
        }

        if (strcmp(key,ptaux->dict->entries[hash]->key) == 0 && (ptaux->dict->entries[hash]->item->operador == operador)){
            return ptaux->dict->entries[hash]->item;
        }

        comp_dict_node_t* current = ptaux->dict->entries[hash];
        do {
            if (strcmp(key,current->item->value) == 0 && (current->item->operador == operador)){
                return current->item;
            }
            current = current->next;
        } while(current != NULL);

        ptaux = ptaux->next;
    }

    return NULL;
}

int encontra_tipo(char* key, int operador){
    int hash = hash_function(key);

    comp_stack_dict_t* ptaux = stack_scope;

    while(ptaux != NULL){
       if ((ptaux->dict->entries[hash] == NULL)){
            ptaux = ptaux->next;
            continue;
        }

        if (strcmp(key,ptaux->dict->entries[hash]->key) == 0 && (ptaux->dict->entries[hash]->item->operador == operador)){
            return ptaux->dict->entries[hash]->item->type_var;
        }

        comp_dict_node_t* current = ptaux->dict->entries[hash];
        do {
            if (strcmp(key,current->item->value) == 0 && (current->item->operador == operador)){
                return current->item->type_var;
            }
            current = current->next;
        } while(current != NULL);

        ptaux = ptaux->next;
    }
    return IKS_TYPE_NOT_DEFINED;
}

int encontra_operador(char* key){
    int hash = hash_function(key);
    comp_stack_dict_t* ptaux = stack_scope;

    while(ptaux != NULL){
        if ((ptaux->dict->entries[hash] == NULL)){
            ptaux = ptaux->next;
            continue;
        }

        if (strcmp(key,ptaux->dict->entries[hash]->key) == 0){
            return ptaux->dict->entries[hash]->item->operador;
        }

        comp_dict_node_t* current = ptaux->dict->entries[hash];
        do {
            if (strcmp(key,current->key) == 0){
                return current->item->operador;
            }
            current = current->next;
        } while(current != NULL);

        ptaux = ptaux->next;
    }
    return -1;
}

int list_count(comp_list_t* list_args){
    int count = 0;
    comp_list_t* aux = list_args;
    while (aux != NULL){
        count++;
        aux = aux->next;
    }

    return count;
}

void list_func_connect(comp_tree_t* tree, comp_list_t* list_args, comp_dict_item_t* hash){
    if (tree != NULL && list_args != NULL){
        tree->list_args = list_args;
        hash->list_args = list_args;
        hash->count_args = list_count(list_args);
    }
}

comp_list_t* list_concat(comp_list_t* list_a, comp_list_t* list_b){
    comp_list_t* list_aux = list_a;
    while (list_aux != NULL){
        if (list_aux->next == NULL){
            list_a->next = list_b;
        }
        list_aux = list_aux->next;
    }

    return list_a;
}

comp_list_t* list_create(comp_dict_item_t* item){
    comp_list_t* new_list = malloc(sizeof(comp_list_t));
    new_list->item = item;
    new_list->next = NULL;
    return new_list;
}
