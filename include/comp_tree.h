typedef struct comp_tree_t {
    int id;
    int type;
    char* lex;
    struct comp_tree_t* children;
    struct comp_tree_t* next_brother;
} comp_tree_t;

comp_tree_t *create_node(int, char*, comp_tree_t*);
comp_tree_t* create_empty_node();


