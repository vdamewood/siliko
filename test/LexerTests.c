#include <criterion/criterion.h>
#include <SilikoCore/Input.h>
#include <SilikoCore/Lexer.h>

#define TestLexer(NAME, INSTRING, ...) \
Test(LexerTests, NAME) \
{ \
    SilikoInput *src = SilikoStringInputCreate(INSTRING); \
    SilikoToken *tokens[] = \
    { \
        __VA_ARGS__ \
    }; \
    SilikoLexer *lex = SilikoLexerCreate(src, -1, -1); \
    for (int i = 0; i < sizeof tokens/sizeof(SilikoToken*); i++) \
    { \
        const SilikoToken *current = SilikoLexerGetToken(lex); \
        cr_assert(SilikoTokenGetStatus(current) == SilikoTokenGetStatus(tokens[i]), \
            "Types[%i]: %i/%c %i/%c", \
            i, \
            SilikoTokenGetStatus(current), \
            SilikoTokenGetStatus(current), \
            SilikoTokenGetStatus(tokens[i]), \
            SilikoTokenGetStatus(tokens[i]) \
        ); \
        switch(SilikoTokenGetStatus(current)) \
        { \
        case SilikoTokenCharacter: \
            cr_assert(SilikoTokenGetCharacter(current) == SilikoTokenGetCharacter(tokens[i])); \
            break; \
        case SilikoTokenInteger: \
            cr_assert(SilikoTokenGetInteger(current) == SilikoTokenGetInteger(tokens[i])); \
            break; \
        case SilikoTokenReal: \
            cr_assert(SilikoTokenGetReal(current) == SilikoTokenGetReal(tokens[i])); \
            break; \
        case SilikoTokenId: \
            cr_assert( \
                strcmp( \
                    SilikoTokenGetId(current), \
                    SilikoTokenGetId(tokens[i])) \
                == 0); \
        default: \
            break; \
        } \
        SilikoLexerAdvance(lex); \
        SilikoTokenDestroy(tokens[i]); \
        tokens[i] = NULL; \
    } \
}

TestLexer(Nothing,
    "",
    SilikoTokenCreateEndOfInput()
)

TestLexer(AnInteger,
    "42386",
    SilikoTokenCreateFromInteger(42386),
    SilikoTokenCreateEndOfInput()
)

TestLexer(AFloat,
    "32156.25",
    SilikoTokenCreateFromReal(32156.25),
    SilikoTokenCreateEndOfInput()
)

TestLexer(AnId,
    "beep",
    SilikoTokenCreateFromId("beep"),
    SilikoTokenCreateEndOfInput()
)

TestLexer(TwoPlusTwo,
    "2 + 2.0",
    SilikoTokenCreateFromInteger(2),
    SilikoTokenCreateFromCharacter('+'),
    SilikoTokenCreateFromReal(2.0),
    SilikoTokenCreateEndOfInput()
)

TestLexer(SomethingComplex,
    "3d6 + -5 * (sin(3.25)/cos(5.125))",
    SilikoTokenCreateFromInteger(3),
    SilikoTokenCreateFromCharacter('d'),
    SilikoTokenCreateFromInteger(6),
    SilikoTokenCreateFromCharacter('+'),
    SilikoTokenCreateFromCharacter('-'),
    SilikoTokenCreateFromInteger(5),
    SilikoTokenCreateFromCharacter('*'),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromId("sin"),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromReal(3.25),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter('/'),
    SilikoTokenCreateFromId("cos"),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromReal(5.125),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateEndOfInput()
)

// No, this won't parse using the existing parser, but you could write
// one yourself if you wanted to.
TestLexer(SomethingComplexLispy,
    "(*\n"
    "\t(+ (d 3 6) -5)\n"
    "\t(/ (sin 3.25)\n"
    "\t\t(cos 5.125)))\n",
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromCharacter('*'),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromCharacter('+'),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromCharacter('d'),
    SilikoTokenCreateFromInteger(3),
    SilikoTokenCreateFromInteger(6),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter('-'),
    SilikoTokenCreateFromInteger(5),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromCharacter('/'),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromId("sin"),
    SilikoTokenCreateFromReal(3.25),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter('('),
    SilikoTokenCreateFromId("cos"),
    SilikoTokenCreateFromReal(5.125),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateFromCharacter(')'),
    SilikoTokenCreateEndOfInput()
)
