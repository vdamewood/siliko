#if !defined PARSERX_H
#define PARSERX_H

struct xvcalc_tree;
struct xvcalc_operation;
struct xvcalc_number;
struct xvcalc_function;

struct xvcalc_tree {
	char type;
	union {
		struct xvcalc_number * num;
		struct xvcalc_operation * op;
		struct xvcalc_function * func;
	};
};

struct xvcalc_operation {
	char type;
	struct xvcalc_tree * left;
	struct xvcalc_tree * right;
};

struct xvcalc_number {
	char type;
	union {
		int i;
		float f;
	};
};

struct xvcalc_function {
	char *name;
	int arg_count;
	void *arg_vector; // TODO: Change this to a list of arguments.
};

struct xvcalc_tree *
xvcalc_new_operation(
	char type,
	struct xvcalc_tree *left,
	struct xvcalc_tree *right);
struct xvcalc_tree * xvcalc_new_int(int value);
struct xvcalc_tree * xvcalc_new_float(float value);

int xvcalc_evaluate_tree(struct xvcalc_tree * tree);
void xvcalc_delete_tree(struct xvcalc_tree * tree);

int   xvcalc_dice(int,int);
void  xvcalc_set_int(int);
void  xvcalc_set_float(float);
void  xvcalc_set_malloc_error(void);
void  vxcalc_report_lex_error(char);
void  xvcalc_yyerror(const char *);

#endif /* PARSERX_H */
