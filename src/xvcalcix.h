#if !defined PARSERX_H
#define PARSERX_H

struct xvcalc_intx_tree;
struct xvcalc_intx_operation;
struct xvcalc_intx_number;
struct xvcalc_intx_function;

struct xvcalc_intx_tree {
	char type;
	union {
		struct xvcalc_intx_number * num;
		struct xvcalc_intx_operation * op;
		struct xvcalc_intx_function * func;
	};
};

struct xvcalc_intx_operation {
	char type;
	struct xvcalc_intx_tree * left;
	struct xvcalc_intx_tree * right;
};

struct xvcalc_intx_number {
	char type;
	union {
		int i;
		float f;
	};
};

struct xvcalc_intx_function {
	char *name;
	int arg_count;
	void *arg_vector; // TODO: Change this to a list of arguments.
};

struct xvcalc_intx_tree *
xvcalc_intx_new_operation(
	char type,
	struct xvcalc_intx_tree *left,
	struct xvcalc_intx_tree *right);
struct xvcalc_intx_tree * xvcalc_intx_new_int(int value);
struct xvcalc_intx_tree * xvcalc_intx_new_float(float value);

int xvcalc_intx_evaluate_tree(struct xvcalc_intx_tree * tree);
void xvcalc_intx_delete_tree(struct xvcalc_intx_tree * tree);

int   xvcalc_intx_dice(int,int);
void  xvcalc_intx_set_int(int);
void  xvcalc_intx_set_float(float);
void  xvcalc_intx_set_malloc_error(void);
void  vxcalc_intx_report_lex_error(char);
void  xvcalc_intx_yyerror(const char *);

#endif /* PARSERX_H */
