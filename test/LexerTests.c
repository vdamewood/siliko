#include <criterion/criterion.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/Lexer.h>

#define TestLexer(NAME, INSTRING, ...) \
Test(LexerTests, NAME) \
{ \
    SilikoDataSource *src = SilikoStringSourceNew(INSTRING); \
    struct SilikoToken tokens[] = \
    { \
        __VA_ARGS__ \
    }; \
    SilikoLexer *lex = SilikoLexerNew(src); \
    for (int i = 0; i < sizeof tokens/sizeof(struct SilikoToken); i++) \
    { \
        struct SilikoToken current = SilikoLexerGetToken(lex); \
        cr_assert(current.Type == tokens[i].Type, \
            "Types[%i]: %i/%c %i/%c", \
            i, \
            current.Type, \
            current.Type, \
            tokens[i].Type, \
            tokens[i].Type \
        ); \
        switch(current.Type) \
        { \
        case SILIKO_TOK_INTEGER: \
            cr_assert(current.Integer == tokens[i].Integer); \
            break; \
        case SILIKO_TOK_FLOAT: \
            cr_assert(current.Float == tokens[i].Float); \
            break; \
        default: \
            break; \
        } \
        SilikoLexerNext(lex); \
    } \
}

TestLexer(Nothing,
    "",
    {SILIKO_TOK_EOL, {.Integer=0}}
)

TestLexer(AnInteger,
    "42386",
    {SILIKO_TOK_INTEGER, {.Integer=42386}}
)

TestLexer(AFloat,
    "32156.25",
    {SILIKO_TOK_FLOAT, {.Float=32156.25}}
)

TestLexer(AnId,
    "beep",
    {SILIKO_TOK_ID, {.Id="beep"}}
)

TestLexer(TwoPlusTwo,
    "2 + 2.0",
    {SILIKO_TOK_INTEGER, {.Integer=2}},
    {SILIKO_TOK_ADDITION, {.Integer=0}},
    {SILIKO_TOK_FLOAT, {.Float=2.0}},
    {SILIKO_TOK_EOL, {.Integer=0}}
)

TestLexer(SomethingComplex,
    "3d6 + -5 * (sin(3.25)/cos(5.125))",
    {SILIKO_TOK_INTEGER, {.Integer=3}},
    {SILIKO_TOK_DICE, {.Integer=0}},
    {SILIKO_TOK_INTEGER, {.Integer=6}},
    {SILIKO_TOK_ADDITION, {.Integer=0}},
    {SILIKO_TOK_SUBTRACT, {.Integer=0}},
    {SILIKO_TOK_INTEGER, {.Integer=5}},
    {SILIKO_TOK_MULTIPLY, {.Integer=0}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_ID, {.Id="sin"}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_FLOAT, {.Float=3.25}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_DIVISION, {.Integer=0}},
    {SILIKO_TOK_ID, {.Id="cos"}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_FLOAT, {.Float=5.125}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_EOL, {.Integer=0}}
)

// No, this won't parse using the existing parser, but you could write
// one yourself if you wanted to.
TestLexer(SomethingComplexLispy,
    "(*\n"
    "\t(+ (d 3 6) -5)\n"
    "\t(/ (sin 3.25)\n"
    "\t\t(cos 5.125)))\n",
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_MULTIPLY, {.Integer=0}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_ADDITION, {.Integer=0}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_DICE, {.Integer=0}},
    {SILIKO_TOK_INTEGER, {.Integer=3}},
    {SILIKO_TOK_INTEGER, {.Integer=6}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_SUBTRACT, {.Integer=0}},
    {SILIKO_TOK_INTEGER, {.Integer=5}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_DIVISION, {.Integer=0}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_ID, {.Id="sin"}},
    {SILIKO_TOK_FLOAT, {.Float=3.25}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_LPAREN, {.Integer=0}},
    {SILIKO_TOK_ID, {.Id="cos"}},
    {SILIKO_TOK_FLOAT, {.Float=5.125}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_RPAREN, {.Integer=0}},
    {SILIKO_TOK_EOL, {.Integer=0}}
)
