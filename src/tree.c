#include <stdlib.h>
#include "xvcalcix.h"


struct xvcalc_intx_tree *
xvcalc_intx_new_operation(
        char type,
        struct xvcalc_intx_tree *left,
        struct xvcalc_intx_tree *right)
{
	return malloc(sizeof(struct xvcalc_intx_tree));
}
struct xvcalc_intx_number * xvcalc_intx_new_int(int value)
{
	return malloc(sizeof(struct xvcalc_intx_number));
}

struct xvcalc_intx_number * xvcalc_intx_new_float(float value)
{
	return malloc(sizeof(struct xvcalc_intx_number));
}

void xvcalc_intx_delete_tree(struct xvcalc_intx_tree * tree)
{
	// Wait to implement.
}

void xvcalc_intx_delete_number(struct xvcalc_intx_number * number)
{
}

void xvcalc_intx_delete_operation(struct xvcalc_intx_operation * operation)
{
}
