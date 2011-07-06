#include <stdlib.h>
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

struct xvcalc_number xvcalc_evaluate_tree(struct xvcalc_tree * tree)
{
	xv_number rVal;
	/* FIXME: set an error */
	if (!tree) return rVal;

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
	}
	return rVal;
}

void xvcalc_delete_tree(struct xvcalc_tree * tree)
{
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
				free(tree->func);
				break;
		}
		free(tree);
	}
}
