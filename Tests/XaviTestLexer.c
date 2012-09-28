#include <stdio.h>

#include "XaviLexer.h"
#include "XaviYyParser.h"

void printResult(int terminal, YYSTYPE token)
{
	printf("Token: ");
	switch (terminal) {
	case EOL:
		printf("EOL\n");
		break;
	case UNEG:
		printf("UNEG\n");
		break;
	case INTEGER:
		printf("Integer(%i)\n", token.i);
		break;
	case FLOAT:
		printf("Float(%f)\n", token.f);
		break;
	case ID:
		printf("ID(%s)\n", token.s);
		break;
	case ERROR:
		printf("Error! (%i)\n", terminal);
		break;
	default:
		printf("Character (%c)\n", terminal);
	}
}

int main(int argc, char *argv[])
{
	int terminal = -1;
	YYSTYPE token;
	if (argc != 2) {
		printf("Usage: %s <expression>", argv[0]);
		return 1;
	}

	XaviLexer * lexer = XaviLexerNew(argv[1]);

	while (terminal != EOL)
	{
		terminal = XaviLexerRead(lexer, &token);
		printResult(terminal, token);
	}
	return 0;
}
