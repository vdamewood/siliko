#include "Xavi.h"
#include "XaviStructs.h"
#include "XaviParser.h"

int XaviInternalParse(
	XaviNumber * value,
	XaviMemoryPool * pool,
	XaviLexer * lexer)
{
	value->status = S_INTEGER;
	value->i = 42;
	return 0;
}
