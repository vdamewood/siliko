#if !defined XVC_STRUCTS_H
#define XVC_STRUCTS_H

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

typedef struct xvcalc_tree      XvcTree;
typedef struct xvcalc_operation XvcOperator;
typedef struct xvcalc_number    XvcNumber;
typedef struct xvcalc_function  XvcFunction;
typedef struct xvcalc_arglist	XvcArglist;

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
	number * args;
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
	number * eval_args;
};

struct xvcalc_arglist {
	int depth;
	tree * value;
	arglist * next;
};

#endif // XVC_STRUCTS_H
