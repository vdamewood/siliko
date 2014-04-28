#if !defined XAVI_PARSER_H
#define XAVI_PARSER_H

#if !defined USE_BISON
#define USE_BISON 0
#endif /* USE_BISON */


#if USE_BISON
#include "XaviYyParser.h"
typedef YYSTYPE XaviTokenValue;
#else
#include "Xavi.h"
#include "XaviStructs.h"

enum yytokentype
{
     EOL = 0,
     UNEG = 258,
     INTEGER = 259,
     FLOAT = 260,
     ID = 261,
     ERROR = 262
};


union XaviTokenValue
{
        XaviTree * t;
        XaviArglist * a;
        char * s;
        int i;
        float f;
};
typedef union XaviTokenValue XaviTokenValue;

#include "XaviLexer.h"

int XaviInternalParse(XaviNumber *, XaviMemoryPool *, XaviLexer *);
#endif /* !USE_BISON */

#endif /* XAVI_PARSER_H */
