#if !defined XVC_STRUCTS_H
#define XVC_STRUCTS_H

struct XvcTree;
struct XvcOperator;
struct XvcNumber;
struct XvcFunction;
struct XvcArglist;

typedef struct XvcTree      XvcTree;
typedef struct XvcOperator  XvcOperator;
typedef struct XvcNumber    XvcNumber;
typedef struct XvcFunction  XvcFunction;
typedef struct XvcArglist	XvcArglist;

struct XvcTree {
	char type;
	union {
		XvcNumber * num;
		XvcOperator * op;
		XvcFunction * func;
	};
};

struct XvcOperator {
	char type;
	XvcTree * left;
	XvcTree * right;
	XvcNumber * args;
};

struct XvcNumber {
	char type;
	union {
		int i;
		float f;
	};
};

struct XvcFunction {
	char * name;
	int arg_count;
	XvcTree ** arg_vector;
};

struct XvcArglist {
	int depth;
	XvcTree * value;
	XvcArglist * next;
};

#endif // XVC_STRUCTS_H
