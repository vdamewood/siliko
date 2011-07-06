#include <stdlib.h>
#include "xvcalcix.h"


struct xvcalc_intx_tree *
xvcalc_intx_new_operation(
        char type,
        struct xvcalc_intx_tree *left,
        struct xvcalc_intx_tree *right)
{
	struct xvcalc_intx_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_intx_tree));
	rVal->op = malloc(sizeof(struct xvcalc_intx_operation));
	rVal->type = 'o';
	rVal->op->type = type;
	rVal->op->left = left;
	rVal->op->right = right;
	return rVal;
}

struct xvcalc_intx_tree * xvcalc_intx_new_int(int value)
{
	struct xvcalc_intx_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_intx_tree));
	rVal->num = malloc(sizeof(struct xvcalc_intx_number));
	rVal->type = 'n';
	rVal->num->type = 'i';
	rVal->num->i = value;
	return rVal;
}

struct xvcalc_intx_tree * xvcalc_intx_new_float(float value)
{
	struct xvcalc_intx_tree * rVal;
	rVal = malloc(sizeof(struct xvcalc_intx_tree));
	rVal->num = malloc(sizeof(struct xvcalc_intx_number));
	rVal->type = 'n';
	rVal->num->type = 'f';
	rVal->num->f = value;
	return rVal;
}

int xvcalc_intx_evaluate_tree(struct xvcalc_intx_tree * tree)
{
	if (!tree) return 0;

	switch (tree->type) {
	case 'n':
		switch (tree->num->type) {
		case 'i':
			return tree->num->i;
		case 'f':
			return (int) tree->num->f;
		}
	case 'o':
		switch (tree->op->type) {
		case '+':
			return xvcalc_intx_evaluate_tree(tree->op->left)
				+ xvcalc_intx_evaluate_tree(tree->op->right);
		case '-':
			return xvcalc_intx_evaluate_tree(tree->op->left)
				- xvcalc_intx_evaluate_tree(tree->op->right);
		case '*':
			return xvcalc_intx_evaluate_tree(tree->op->left)
				* xvcalc_intx_evaluate_tree(tree->op->right);
		case '/':
			return xvcalc_intx_evaluate_tree(tree->op->left)
				/ xvcalc_intx_evaluate_tree(tree->op->right);
		case 'd':
			return xvcalc_intx_dice(
				xvcalc_intx_evaluate_tree(tree->op->left),
				xvcalc_intx_evaluate_tree(tree->op->right));
		}
	case 'f':
		return 1;
	}
}

void xvcalc_intx_delete_tree(struct xvcalc_intx_tree * tree)
{
	if (tree) {
		switch (tree->type) {
			case 'n':
				free(tree->num);
				break;
			case 'o':
				xvcalc_intx_delete_tree(tree->op->left);
				xvcalc_intx_delete_tree(tree->op->right);
				free(tree->op);
				break;
			case 'f':
				free(tree->func);
				break;
		}
		free(tree);
	}
}
