#include <criterion/criterion.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/Lexer.h>

#define TestLexer(NAME, INSTRING, ...) \
Test(LexerTests, NAME) \
{ \
    SilikoDataSource *src = SilikoStringSourceNew(INSTRING); \
    SilikoToken *tokens[] = \
    { \
        __VA_ARGS__ \
    }; \
    SilikoLexer *lex = SilikoLexerNew(src); \
    for (int i = 0; i < sizeof tokens/sizeof(SilikoToken*); i++) \
    { \
        const SilikoToken *current = SilikoLexerGetCurrent(lex); \
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
        SilikoTokenDelete(tokens[i]); \
        tokens[i] = NULL; \
    } \
}

TestLexer(Nothing,
    "",
    SilikoTokenNewEndOfInput()
)

TestLexer(AnInteger,
    "42386",
    SilikoTokenNewInteger(42386),
    SilikoTokenNewEndOfInput()
)

TestLexer(AFloat,
    "32156.25",
    SilikoTokenNewReal(32156.25),
    SilikoTokenNewEndOfInput()
)

TestLexer(AnId,
    "beep",
    SilikoTokenNewId("beep"),
    SilikoTokenNewEndOfInput()
)

TestLexer(TwoPlusTwo,
    "2 + 2.0",
    SilikoTokenNewInteger(2),
    SilikoTokenNewCharacter('+'),
    SilikoTokenNewReal(2.0),
    SilikoTokenNewEndOfInput()
)

TestLexer(SomethingComplex,
    "3d6 + -5 * (sin(3.25)/cos(5.125))",
    SilikoTokenNewInteger(3),
    SilikoTokenNewCharacter('d'),
    SilikoTokenNewInteger(6),
    SilikoTokenNewCharacter('+'),
    SilikoTokenNewCharacter('-'),
    SilikoTokenNewInteger(5),
    SilikoTokenNewCharacter('*'),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewId("sin"),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewReal(3.25),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter('/'),
    SilikoTokenNewId("cos"),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewReal(5.125),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewEndOfInput()
)

// No, this won't parse using the existing parser, but you could write
// one yourself if you wanted to.
TestLexer(SomethingComplexLispy,
    "(*\n"
    "\t(+ (d 3 6) -5)\n"
    "\t(/ (sin 3.25)\n"
    "\t\t(cos 5.125)))\n",
    SilikoTokenNewCharacter('('),
    SilikoTokenNewCharacter('*'),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewCharacter('+'),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewCharacter('d'),
    SilikoTokenNewInteger(3),
    SilikoTokenNewInteger(6),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter('-'),
    SilikoTokenNewInteger(5),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewCharacter('/'),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewId("sin"),
    SilikoTokenNewReal(3.25),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter('('),
    SilikoTokenNewId("cos"),
    SilikoTokenNewReal(5.125),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewCharacter(')'),
    SilikoTokenNewEndOfInput()
)
