#include <stdlib.h>
#include <string.h>
#include "xvcalcix.h"


struct xvcalc_tree *
xvcalc_new_operation(
        char type,
        struct xvcalc_tree *left,
        struct xvcalc_tree *right)
{
	struct xvcalc_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_tree));
	rVal->op = malloc(sizeof(struct xvcalc_operation));
	rVal->type = 'o';
	rVal->op->type = type;
	rVal->op->left = left;
	rVal->op->right = right;
	return rVal;
}

struct xvcalc_tree * xvcalc_new_int(int value)
{
	struct xvcalc_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_tree));
	rVal->num = malloc(sizeof(struct xvcalc_number));
	rVal->type = 'n';
	rVal->num->type = 'i';
	rVal->num->i = value;
	return rVal;
}

struct xvcalc_tree * xvcalc_new_float(float value)
{
	struct xvcalc_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_tree));
	rVal->num = malloc(sizeof(struct xvcalc_number));
	rVal->type = 'n';
	rVal->num->type = 'f';
	rVal->num->f = value;
	return rVal;
}

void xvcalc_arglist_to_array(tree ** array, arglist * in_arglist)
{
	*array = in_arglist->value;
	if (in_arglist->next)
		xvcalc_arglist_to_array(array+1, in_arglist->next);
	free(in_arglist);
}

tree * xvcalc_new_function(char * name, arglist * in_arglist)
{
	struct xvcalc_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_tree));
	rVal->type = 'f';

	rVal->func = malloc(sizeof(xv_function));
	rVal->func->name = malloc(strlen(name) + 1);
	strcpy(rVal->func->name, name);
	
	if(in_arglist) {
		rVal->func->arg_count = in_arglist->depth;
		rVal->func->arg_vector = malloc(sizeof(tree *) * in_arglist->depth);
		xvcalc_arglist_to_array(rVal->func->arg_vector, in_arglist);
	}
	else {
		rVal->func->arg_count = 0;
		rVal->func->arg_vector = NULL;
	}
	
	return rVal;
}

xv_number xvcalc_add(xv_number left, xv_number right)
{
	xv_number rVal;
	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f + right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f + (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i + right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i + right.i;
		}
	}
	return rVal;
}

xv_number xvcalc_sub(xv_number left, xv_number right)
{
	xv_number rVal;
	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f - right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f - (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i - right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i - right.i;
		}
	}
	return rVal;
}

xv_number xvcalc_mul(xv_number left, xv_number right)
{
	xv_number rVal;
	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f * right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f * (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i * right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i * right.i;
		}
	}
	return rVal;
}

xv_number xvcalc_div(xv_number left, xv_number right)
{
	// TOOD: Add division by zero check.
	xv_number rVal;
	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f / right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f / (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i / right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i / right.i;
		}
	}
	return rVal;
}

xv_number xvcalc_dice(xv_number n_count, xv_number n_faces)
{
	xv_number rVal;
	int count;
	int faces;
	if (n_count.type == 'f') count = (int) n_count.f;
	else count = n_count.i;
	faces = n_faces.i;

	static int hasSeeded = 0;
	int i;
	int running = 0;
	if (!hasSeeded)
		srand(time(NULL));

	for (i = 1; i <= count; i++)
		running += (rand() % faces) + 1;
	rVal.type = 'i';
	rVal.i = running;
	return rVal;
}

number xvcalc_evaluate_function(char * name, int count, number * arguments)
{
	number rVal;
	rVal = arguments[0];
	if (strcmp(name, "abs") == 0) {
		if (rVal.type == 'f' && rVal.f < 0.0)
			rVal.f = rVal.f * -1.0;
		else if (rVal.type == 'i' && rVal.i < 0)
			rVal.i = rVal.i * -1;
	}
	return rVal;
}

struct xvcalc_number xvcalc_evaluate_tree(struct xvcalc_tree * tree)
{
	xv_number rVal;
	int i;
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
			evaluated_arguments =
				malloc(sizeof(number) * tree->func->arg_count);
			for(i = 0; i < tree->func->arg_count; i++) {
				evaluated_arguments[i] = xvcalc_evaluate_tree(
					tree->func->arg_vector[i]);
			}
		}
		rVal = xvcalc_evaluate_function(tree->func->name,
					 tree->func->arg_count,
					 evaluated_arguments);
		if (tree->func->arg_count) {
			for(i = 0; i < tree->func->arg_count; i++) {
				/*free(evaluated_arguments[i]);*/
			}

			free(evaluated_arguments);
			evaluated_arguments = NULL;
		}
	}
		
	return rVal;
}

void xvcalc_delete_tree(struct xvcalc_tree * tree)
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
				free(tree->func);
				break;
		}
		free(tree);
	}
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
