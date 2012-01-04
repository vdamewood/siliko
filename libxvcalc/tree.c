#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

#include "xvcalc.h"
#include "tree.h"
#include "XvcFunctions.h"
#include "XvcOperators.h"
#include "xvcalcix.h"
#include "cleanup.h"

static jmp_buf escape;

void xvcalc_set_value_from_tree(tree * in_tree)
{
	xvcalc_status jmp_value = 0;
	if ((jmp_value = setjmp(escape)) == 0) {
		xvcalc_set_value(xvcalc_evaluate_tree(in_tree));
	}
	else {
		xvcalc_set_status(jmp_value);
	}
}

number xvcalc_evaluate_tree(tree * tree)
{
	number rVal;
	int i;
	if (!tree) longjmp(escape, E_SYNTAX);

	switch (tree->type) {
	case 'n':
		rVal = *(tree->num);
		break;
	case 'o':
		tree->op->args[0] = xvcalc_evaluate_tree(tree->op->left);
		tree->op->args[1] = xvcalc_evaluate_tree(tree->op->right);

		rVal = xvcalc_call_operator(tree->op->type,
									tree->op->args,
									escape);
		
		break;
	case 'f':
		if (tree->func->arg_count) {
			for(i = 0; i < tree->func->arg_count; i++) {
				tree->func->eval_args[i] = xvcalc_evaluate_tree(
					tree->func->arg_vector[i]);
			}
		}

		rVal = xvcalc_call_function(tree->func->name,
									tree->func->arg_count,
									tree->func->eval_args,
									escape);
	}
		
	return rVal;
}

void xvcalc_clear_memory(tree * in_tree)
{
	if (in_tree) {
		xvcalc_delete_tree(in_tree);
		xvcalc_release_dangling_tree(in_tree);
	}
}
