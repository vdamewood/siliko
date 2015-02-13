/*
 * TestXaviLexer.c: Program to test lexer for Xavi.
 * Copyright 2012, 2015 Vincent Damewood
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
#include <string.h>

#include <Xavi/StringSource.h>
#include <Xavi/DataSource.h>
#include <Xavi/Lexer.h>

void PrintToken(XaviToken MyToken)
{
	switch (MyToken.Type)
	{
	case XAVI_TOK_EOL:
		printf("EOL\n");
		break;
	case XAVI_TOK_INTEGER:
		printf("Integer(%d)\n", MyToken.Integer);
		break;
	case XAVI_TOK_FLOAT:
		printf("Float(%f)\n", MyToken.Float);
		break;
	case XAVI_TOK_ID:
		printf("String(%s)\n", MyToken.Id);
		break;
	case XAVI_TOK_ERROR:
		printf("Error");
		break;
	default:
		printf("Operator(%c)\n", (char)MyToken.Type);
	}
}


int main(int argc, char *argv[])
{
	XaviLexer *MyLexer = XaviLexerNew(
		XaviStringSourceNew(
			"abs(2) + 300 / 3d6 + 5.25 * sin(200) - 5"));

	PrintToken(MyLexer->Token);
	if (MyLexer->Token.Type != XAVI_TOK_ID
		|| strcmp(MyLexer->Token.Id, "abs") != 0)
	{
		printf("Failed, expecting: ID(abs)\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);
	if (MyLexer->Token.Type != XAVI_TOK_LPAREN)
	{
		printf("Failed, expecting: (\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);
	if (MyLexer->Token.Type != XAVI_TOK_INTEGER
		|| MyLexer->Token.Integer != 2)
	{
		printf("Failed, expecting: Integer: 2\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_RPAREN)
	{
		printf("Failed, expecting: )\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_ADDITION)
	{
		printf("Failed, expecting: +\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_INTEGER
		|| MyLexer->Token.Integer != 300)
	{
		printf("Failed, expecting: Integer: 300\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_DIVISION)
	{
		printf("Failed, expecting: /\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_INTEGER
		|| MyLexer->Token.Integer != 3)
	{
		printf("Failed, expecting: Integer: 3\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_DICE)
	{
		printf("Failed, expecting: d\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_INTEGER
		|| MyLexer->Token.Integer != 6)
	{
		printf("Failed, expecting: Integer: 6\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_ADDITION)
	{
		printf("Failed, expecting: +\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_FLOAT
		|| MyLexer->Token.Float != 5.25)
	{
		printf("Failed, expecting: Integer: 5.25\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	//Character(*)
	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_MULTIPLY)
	{
		printf("Failed, expecting: *\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	//ID(sin)
	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_ID
		|| strcmp(MyLexer->Token.Id, "sin") != 0)
	{
		printf("Failed, expecting: ID(sin)\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_LPAREN)
	{
		printf("Failed, expecting: (\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_INTEGER
		|| MyLexer->Token.Integer != 200)
	{
		printf("Failed, expecting: Integer: 200\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_RPAREN)
	{
		printf("Failed, expecting: )\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_SUBTRACT)
	{
		printf("Failed, expecting: -\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_INTEGER
		|| MyLexer->Token.Integer != 5)
	{
		printf("Failed, expecting: Integer: 5\n");
		return 1;
	}
	XaviLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != XAVI_TOK_EOL)
	{
		printf("Failed, expecting: EOL\n");
		return 1;
	}

	printf("Total Success!\n");
	return 0;
}
