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
        fprintf(stderr, "aux children %s type-iks %d\n", aux_children->lex, aux_children->type);

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
            /*fprintf(stdout, "erro input\n");*/
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
        /*fprintf(stderr, "aux children %s %d\n", aux_children->lex, aux_children->type);*/
        if (aux_children->lex == label){
            /*fprintf(stdout, "encontrou func\n");*/
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
        /*fprintf(stderr, "aux children %s %d\n", aux_children->lex, aux_children->type);*/
        if (aux_children->type == type){
            /*fprintf(stdout, "encontrou type\n");*/
            /*fprintf(stdout, "%d\n", aux_children->type);*/
            /*fprintf(stdout, "%d\n", node->type);*/
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

    /*fprintf(stdout, "entrou ver return\n");*/
    /*print_syntax_tree(node);*/

    // Encontra a função na árvore
    node_aux_label = find_node_by_label(node_aux, label_function);
    /*fprintf(stdout, "node l type %d %s\n", node_aux_label->type, node_aux_label->lex);*/
    // Encontra o return na árvore
    node_aux_type = find_node_by_type(node_aux_label, IKS_AST_RETURN);

    /*if (node_aux_type != NULL && node_aux_type->children != NULL){*/
        /*fprintf(stdout, "node next_brother type %d\n", node_aux_type->children->type);*/
    /*}*/
    /*return;*/

    /*if (node_aux_type != NULL){*/
    if (node_aux_type != NULL && node_aux_type->children != NULL){
        /*fprintf(stdout, "node t type %d\n", node_aux_type->children->type);*/
        /*fprintf(stdout, "node t type_var %d\n", node_aux_type->children->hash->type_var);*/
        /*fprintf(stdout, "encontrou\n");*/

        /*fprintf(stdout, "var return %d\n", var_return->children->hash->type_var);*/
        if (node_aux_type->children->hash->type_var != type_var_function) {
            /*printf("test wrong type return\n");*/
            exit(IKS_ERROR_WRONG_PAR_RETURN);
        }
    }
    else {
        /*fprintf(stdout, "não encontrou\n");*/
    }
}

// Verifica o número e o tipo dos argumentos
void verifica_argumentos(comp_tree_t* node, char* label_function, int empty){
    comp_tree_t* node_aux = node;
    comp_tree_t* node_aux_label = NULL;
    comp_tree_t* node_aux_type = NULL;

    // Conta o número de parâmetros na declaração da função
    if (!empty){

    }

    print_stack_dict(stack_scope);
    fprintf(stdout, "entrou ver argumentos\n");
    print_syntax_tree(node);

    // Encontra a chamada da função na árvore
    node_aux_label = find_node_by_label(node_aux, label_function);
    fprintf(stdout, "node l type %d %s\n", node_aux_label->type, node_aux_label->lex);
    return;

    // Conta o número de parâmetros da chamada da função
    if (node_aux_label != NULL){

    }

}

void verifica_atribuicao(comp_tree_t* node,int tipo){
    
   // fprintf(stdout, "teste key %s\n", node->hash->key);
  
    int operador = encontra_operador(node->hash->key);
   // fprintf(stdout,"operador %d",operador);
    //getchar();
    //print_stack_dict(stack_scope);
    //getchar();
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
	if(tipo == IKS_STRING){//Variável tipo STRING lado esquerdo
	  if(tipo_funcao  == IKS_INT){
		exit(IKS_ERROR_WRONG_TYPE);
	    }
	}
      }
    }
    
    if(operador == USO_VETOR_INDEXADO){
      
    }
    
    if(operador == USO_LITERAL){
  //    fprintf(stdout,"pêlê");
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
  

int encontra_tipo(char* key, int operador){
    
    int hash = hash_function(key);
    
    fprintf(stdout, "Vamos ver o key2 => %s\n\n",key);

    comp_stack_dict_t* ptaux = stack_scope;
    
    while(ptaux != NULL){
       if ((ptaux->dict->entries[hash] == NULL)){
	    ptaux = ptaux->next;
            continue;
        }
	fprintf(stdout, "Vamos ver o key => %s\n\n",ptaux->dict->entries[hash]->key);
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