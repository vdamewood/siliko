/*
 * XaviTestLexer.c: Program to test lexer for Xavi.
 * Copyright 2012 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
