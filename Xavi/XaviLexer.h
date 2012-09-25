#if !defined XAVI_LEXER_H
#define XAVI_LEXER_H

#if !defined USE_BISON
#define USE_BISON 0
#endif /* USE_BISON */

struct XaviLexer
{
	const char * input;
	const char * current;
	char * lexeme;
};

typedef struct XaviLexer XaviLexer;
typedef XaviLexer * yyscan_t;

#if USE_BISON
#include "XaviYyParser.h"
#else
#error "Building without BISON is currently not supported."
#endif


int XaviLexerRead(XaviLexer *, YYSTYPE *);
void XaviLexerDestroy(XaviLexer**);
XaviLexer * XaviLexerNew(const char * inputString);
#define Xavi_yylex(a,b) XaviLexerRead((b), (a))
#endif
