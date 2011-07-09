#if !defined STRUCTS_H
#define STRUCTS_H

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

#endif /* STRUCTS_H */
