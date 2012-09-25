#if !defined XAVI_LEXER_H
#define XAVI_LEXER_H

#if !defined USE_BISON
#define USE_BISON 0
#endif /* USE_BISON */

#if USE_BISON
#if !defined YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
#include "XaviYyParser.h"
#else
#error "Building without BISON is currently not supported."
#endif

struct XaviLexer
{
	const char * input;
	const char * current;
	char * lexeme;
};

typedef struct XaviLexer XaviLexer;

int XaviLexerRead(yyscan_t, YYSTYPE *);
void XaviLexerDestroy(XaviLexer**);
XaviLexer * XaviLexerNew(const char * inputString);
#define Xavi_yylex(a,b) XaviLexerRead((b), (a))
#endif
