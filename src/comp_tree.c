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
        new_node->hash->operador = type;
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

    /*print_syntax_tree(new_node);*/
    return new_node;
}

void connect_nodes(comp_tree_t* node_a, comp_tree_t* node_b){
    comp_tree_t* aux;

    /*fprintf(stderr, "Connect nodes\n");*/
    if (node_a != NULL && node_b != NULL){
        // Lógica de conexão da árvore
        if (node_a->children == NULL){
            node_a->children = node_b;
        }
        else {
            /*fprintf(stderr, "node_a children not empty\n");*/
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
        fprintf(stderr, "aux children %s %d\n", aux_children->lex, aux_children->type);

       if (aux_children->next_brother != NULL)
            print_syntax_tree(aux_children->next_brother);

        aux_children = aux_children->children;
    }
}

comp_tree_t* create_empty_node(){
    comp_tree_t* new_node = (comp_tree_t *) malloc(sizeof(comp_tree_t));
    new_node->type = -1;
    new_node->id = -1;
    new_node->lex = NULL;
    new_node->hash = NULL;
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

comp_tree_t* find_return(comp_tree_t* syntax_tree){
    comp_tree_t* aux_brother;
    comp_tree_t* aux_children;

    aux_children = syntax_tree;
    if (aux_children != NULL){

        if (aux_children->type == IKS_AST_RETURN)
            return aux_children;

        if (aux_children->next_brother != NULL)
            find_return(aux_children->next_brother);

        aux_children = aux_children->children;
    }
}


// Verifica se o return corresponde com o tipo da função
void verifica_return(comp_tree_t* node, int type_var_function){
    /*print_syntax_tree(node);*/
    // Encontra o return na árvore
    comp_tree_t* var_return = find_return(node);
    /*print_syntax_tree(var_return);*/

    fprintf(stdout, "var return %d\n", var_return->children->hash->type_var);
    if (var_return->children->hash->type_var != type_var_function) {
        printf("test wrong type return\n");
        exit(IKS_ERROR_WRONG_PAR_RETURN);
    }
}

void verifica_tipo(comp_tree_t* node,int tipo){

    int operador = encontra_operador(node->hash->value);

    fprintf(stdout,"uso var %d s###\n", operador);
    if(operador == USO_VARIAVEL){
        fprintf(stdout,"safado\n");
        int tipo_variavel = encontra_tipo(node->hash->value);
        if(tipo == tipo_variavel){
            fprintf(stdout,"pele\n");
        }
    }
}

int encontra_tipo(char* key){

    int hash = hash_function(key);

    comp_stack_dict_t* ptaux = stack_scope;

    while(ptaux != NULL){
        if ((ptaux->dict->entries[hash] == NULL)){
            ptaux = ptaux->next;
            continue;
        }

        if (strcmp(key,ptaux->dict->entries[hash]->item->value) == 0){
            return ptaux->dict->entries[hash]->item->type_var;
        }

        comp_dict_node_t* current = ptaux->dict->entries[hash];
        do {
            if (strcmp(key,current->item->value) == 0){
                return current->item->type_var;
            }
            current = current->next;
        } while(current != NULL);

        ptaux = ptaux->next;
    }
    return -1;
}

int encontra_operador(char* key){
    int hash = hash_function(key);
    comp_stack_dict_t* ptaux = stack_scope;

    while(ptaux != NULL){
        if ((ptaux->dict->entries[hash] == NULL)){
            ptaux = ptaux->next;
            continue;
        }

        if (strcmp(key,ptaux->dict->entries[hash]->item->value) == 0){
            return ptaux->dict->entries[hash]->item->operador;
        }

        comp_dict_node_t* current = ptaux->dict->entries[hash];
        do {
            if (strcmp(key,current->item->value) == 0){
                return current->item->operador;
            }
            current = current->next;
        } while(current != NULL);

        ptaux = ptaux->next;
    }
    return -1;
}
