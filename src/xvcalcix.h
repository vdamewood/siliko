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

typedef struct xvcalc_tree      xv_tree;
typedef struct xvcalc_operation xv_operation;
typedef struct xvcalc_number    xv_number;
typedef struct xvcalc_function  xv_function;

struct xvcalc_tree *
xvcalc_new_operation(
	char type,
	struct xvcalc_tree *left,
	struct xvcalc_tree *right);
struct xvcalc_tree * xvcalc_new_int(int value);
struct xvcalc_tree * xvcalc_new_float(float value);

xv_number xvcalc_evaluate_tree(struct xvcalc_tree * tree);
void xvcalc_delete_tree(struct xvcalc_tree * tree);

void  xvcalc_set_nil();
void  xvcalc_set_value(xv_number);
void  xvcalc_set_malloc_error(void);
void  vxcalc_report_lex_error(char);
void  xvcalc_yyerror(const char *);

#endif /* PARSERX_H */
