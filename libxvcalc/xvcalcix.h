#if !defined PARSERX_H
#define PARSERX_H

struct xvcalc_tree;
struct xvcalc_operation;
struct xvcalc_number;
struct xvcalc_function;
struct xvcalc_arglist;

typedef struct xvcalc_tree      tree;
typedef struct xvcalc_operation operation;
typedef struct xvcalc_number    number;
typedef struct xvcalc_function  function;
typedef struct xvcalc_arglist	arglist;

struct xvcalc_tree {
	char type;
	union {
		number * num;
		operation * op;
		function * func;
	};
};

struct xvcalc_operation {
	char type;
	tree * left;
	tree * right;
};

struct xvcalc_number {
	char type;
	union {
		int i;
		float f;
	};
};

struct xvcalc_function {
	char * name;
	int arg_count;
	tree ** arg_vector;
};

struct xvcalc_arglist {
	int depth;
	tree * value;
	arglist * next;
};

tree * xvcalc_new_operation(char, tree *, tree *);
tree * xvcalc_new_int(int);
tree * xvcalc_new_float(float);
tree * xvcalc_new_function(char *, arglist *);

number xvcalc_evaluate_tree(tree *);
void xvcalc_delete_tree(tree *);

arglist * xvcalc_add_argument(tree *, arglist *);

void  xvcalc_set_nil();
void  xvcalc_set_value(number);
void  xvcalc_set_malloc_error(void);
void  vxcalc_report_lex_error(char);
void  xvcalc_yyerror(const char *);

#endif /* PARSERX_H */
