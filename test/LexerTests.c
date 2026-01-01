#include <criterion/criterion.h>
#include <SilikoCore/Input.h>
#include <SilikoCore/StringInput.h>
#include <SilikoCore/Lexer.h>

#define TestLexer(NAME, INSTRING, ...) \
Test(LexerTests, NAME) \
{ \
    SilikoInput *src = SilikoStringInputNew(INSTRING); \
    SilikoToken *tokens[] = \
    { \
        __VA_ARGS__ \
    }; \
    SilikoLexer *lex = SilikoLexerNew(src); \
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
    SilikoTokenNewFromInteger(42386),
    SilikoTokenNewEndOfInput()
)

TestLexer(AFloat,
    "32156.25",
    SilikoTokenNewFromReal(32156.25),
    SilikoTokenNewEndOfInput()
)

TestLexer(AnId,
    "beep",
    SilikoTokenNewFromId("beep"),
    SilikoTokenNewEndOfInput()
)

TestLexer(TwoPlusTwo,
    "2 + 2.0",
    SilikoTokenNewFromInteger(2),
    SilikoTokenNewFromCharacter('+'),
    SilikoTokenNewFromReal(2.0),
    SilikoTokenNewEndOfInput()
)

TestLexer(SomethingComplex,
    "3d6 + -5 * (sin(3.25)/cos(5.125))",
    SilikoTokenNewFromInteger(3),
    SilikoTokenNewFromCharacter('d'),
    SilikoTokenNewFromInteger(6),
    SilikoTokenNewFromCharacter('+'),
    SilikoTokenNewFromCharacter('-'),
    SilikoTokenNewFromInteger(5),
    SilikoTokenNewFromCharacter('*'),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromId("sin"),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromReal(3.25),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter('/'),
    SilikoTokenNewFromId("cos"),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromReal(5.125),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewEndOfInput()
)

// No, this won't parse using the existing parser, but you could write
// one yourself if you wanted to.
TestLexer(SomethingComplexLispy,
    "(*\n"
    "\t(+ (d 3 6) -5)\n"
    "\t(/ (sin 3.25)\n"
    "\t\t(cos 5.125)))\n",
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromCharacter('*'),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromCharacter('+'),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromCharacter('d'),
    SilikoTokenNewFromInteger(3),
    SilikoTokenNewFromInteger(6),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter('-'),
    SilikoTokenNewFromInteger(5),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromCharacter('/'),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromId("sin"),
    SilikoTokenNewFromReal(3.25),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter('('),
    SilikoTokenNewFromId("cos"),
    SilikoTokenNewFromReal(5.125),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewFromCharacter(')'),
    SilikoTokenNewEndOfInput()
)
