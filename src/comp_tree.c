#include <stdio.h>
#include <stdlib.h>
#include "comp_tree.h"
#include "gv.h"

int count_nodes = 0;

comp_tree_t* create_node(int type, char* lex, comp_tree_t* node){
    comp_tree_t* aux;
    comp_tree_t* new_node;

    // Create new node
    new_node = create_empty_node();
    new_node->type = type;
    new_node->id = count_nodes;
    count_nodes++;
    gv_declare(type, new_node, lex);

    // Connect nodes
    new_node->lex = lex;
    new_node->children = node;

    aux = node;
    while (aux != NULL){
        gv_connect(new_node, aux);
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
        gv_connect(node_a, node_b);
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
// 	  free(aux);
// 	  return;
//     }
  
  
  //	Segunda versão tenando ir em profundidade primeiro
  //	Também não foi
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
//                 free(aux_children);
//                 return;
//             }
//             else {
//                 free(aux_children->lex);
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

        /*aux_brother = syntax_tree->next_brother;*/
        aux_brother = aux_children->next_brother;
        while (aux_brother != NULL){
            fprintf(stderr, "aux brother %s %d\n", aux_brother->lex, aux_children->type);

            aux_brother = aux_brother->next_brother;
        }

        aux_children = aux_children->children;
    }
}

comp_tree_t* create_empty_node(){
    comp_tree_t* new_node = (comp_tree_t *) malloc(sizeof(comp_tree_t));
    new_node->type = -1;
    new_node->id = -1;
    new_node->lex = NULL;
    new_node->children = NULL;
    new_node->next_brother = NULL;

    return new_node;
}
