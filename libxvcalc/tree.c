#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "operators.h"
#include "functions.h"
#include "xvcalcix.h"

tree * xvcalc_new_operation(char type, tree * left, tree * right)
{
	tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->op = malloc(sizeof(operation));
	rVal->type = 'o';
	rVal->op->type = type;
	rVal->op->left = left;
	rVal->op->right = right;
	return rVal;
}

tree * xvcalc_new_int(int value)
{
	tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->num = malloc(sizeof(number));
	rVal->type = 'n';
	rVal->num->type = 'i';
	rVal->num->i = value;
	return rVal;
}

tree * xvcalc_new_float(float value)
{
	struct xvcalc_tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->num = malloc(sizeof(number));
	rVal->type = 'n';
	rVal->num->type = 'f';
	rVal->num->f = value;
	return rVal;
}

tree * xvcalc_new_function(char * name, arglist * in_arglist)
{
	tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->type = 'f';

	rVal->func = malloc(sizeof(function));
	rVal->func->name = malloc(strlen(name) + 1);
	strcpy(rVal->func->name, name);
	
	if(in_arglist) {
		rVal->func->arg_count = in_arglist->depth;
		rVal->func->arg_vector = malloc(sizeof(tree *) * in_arglist->depth);
		xvcalc_arglist_to_array(rVal->func->arg_vector, in_arglist);
		rVal->func->eval_args = malloc(sizeof(number) * rVal->func->arg_count);
	}
	else {
		rVal->func->arg_count = 0;
		rVal->func->arg_vector = NULL;
		rVal->func->eval_args = NULL;
	}
	
	return rVal;
}

number xvcalc_evaluate_tree(tree * tree)
{
	number rVal;
	int i;
	function_ptr func;
	/* FIXME: set an error */
	if (!tree) return rVal;
	number * evaluated_arguments = NULL;

	switch (tree->type) {
	case 'n':
		rVal = *(tree->num);
		break;
	case 'o':
		switch (tree->op->type) {
		case '+':
			rVal = xvcalc_add(xvcalc_evaluate_tree(tree->op->left),
				xvcalc_evaluate_tree(tree->op->right));
			break;
		case '-':
			rVal = xvcalc_sub(xvcalc_evaluate_tree(tree->op->left),
				xvcalc_evaluate_tree(tree->op->right));
			break;
		case '*':
			rVal = xvcalc_mul(xvcalc_evaluate_tree(tree->op->left),
				xvcalc_evaluate_tree(tree->op->right));
			break;
		case '/':
			rVal = xvcalc_div(xvcalc_evaluate_tree(tree->op->left),
				xvcalc_evaluate_tree(tree->op->right));
			break;
		case 'd':
			rVal = xvcalc_dice(xvcalc_evaluate_tree(tree->op->left),
				xvcalc_evaluate_tree(tree->op->right));
			break;
		}
		break;
	case 'f':
		if (tree->func->arg_count) {
			for(i = 0; i < tree->func->arg_count; i++) {
				tree->func->eval_args[i] = xvcalc_evaluate_tree(
					tree->func->arg_vector[i]);
			}
		}

		func = get_function(tree->func->name);
		rVal = func(tree->func->arg_count, tree->func->eval_args);
	}
		
	return rVal;
}

void xvcalc_delete_tree(tree * tree)
{
	int i;
	if (tree) {
		switch (tree->type) {
			case 'n':
				free(tree->num);
				break;
			case 'o':
				xvcalc_delete_tree(tree->op->left);
				xvcalc_delete_tree(tree->op->right);
				free(tree->op);
				break;
			case 'f':
				free(tree->func->name);
				for (i = 0; i <  tree->func->arg_count; i++) {
					xvcalc_delete_tree(
						tree->func->arg_vector[i]);
				}
				free(tree->func->arg_vector);
				free(tree->func->eval_args);
				free(tree->func);
				break;
		}
		free(tree);
	}
}

void xvcalc_arglist_to_array(tree ** array, arglist * in_arglist)
{
	*array = in_arglist->value;
	if (in_arglist->next)
		xvcalc_arglist_to_array(array+1, in_arglist->next);
	free(in_arglist);
}

arglist * xvcalc_add_argument(tree * new_arg, arglist * old_list)
{
	arglist * rVal;
	rVal = malloc(sizeof(arglist));
	if (old_list) rVal->depth = old_list->depth + 1;
	else rVal->depth = 1;
	rVal->value = new_arg;
	rVal->next = old_list;
	return rVal;
}
