#if !defined TREE_H
#define TREE_H
#include "structs.h"

tree * xvcalc_new_operation(char, tree *, tree *);
tree * xvcalc_new_int(int);
tree * xvcalc_new_float(float);
tree * xvcalc_new_function(char *, arglist *);
void  xvcalc_set_value_from_tree(tree *);
number xvcalc_evaluate_tree(tree *);
void xvcalc_delete_tree(tree *);

arglist * xvcalc_add_argument(tree *, arglist *);
void xvcalc_delete_arglist(arglist *);
void xvcalc_arglist_to_array(tree ** array, arglist * in_arglist);

char * xvcalc_make_id(char * in_token);
void xvcalc_delete_id(char * in_token);
#endif /* TREE_H */