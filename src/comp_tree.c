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

    return new_node;
}

void connect_nodes(comp_tree_t* node_a, comp_tree_t* node_b){
    if (node_a == NULL){
        /*fprintf(stderr, "node_a\n");*/
    }
    else {
        comp_tree_t* aux = node_a;
        while (aux != NULL){
            /*gv_connect(new_node, aux);*/
            /*fprintf(stderr, "a %s\n", aux->lex);*/
            aux = aux->next_brother;
        }
    }

    if (node_b == NULL){
        /*fprintf(stderr, "node_b\n");*/
    }
    else {
        comp_tree_t* aux = node_b;
        while (aux != NULL){
            /*gv_connect(new_node, aux);*/
            /*fprintf(stderr, "b %s\n", aux->lex);*/
            aux = aux->next_brother;
        }
    }

    /*fprintf(stderr,"out\n");*/

    if (node_a != NULL && node_b != NULL){
        gv_connect(node_a, node_b);
    }
    /*gv_connect(node_a, node_b);*/
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

/*int main(){*/
    /*comp_tree_t *node1;*/
    /*comp_tree_t node2;*/

    /*node2.type = 2;*/
    /*node1 = create_node(1, "test", &node2);*/

    /*printf("%d\n", node1->type);*/
    /*printf("%d\n", node2.type);*/

    /*return 0;*/
/*}*/
