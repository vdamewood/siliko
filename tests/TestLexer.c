/* TestLexer.c: Program to test lexer for Siliko.
 * Copyright 2012, 2015, 2016 Vincent Damewood
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

#include "StringSource.h"
#include "DataSource.h"
#include "Lexer.h"

void PrintToken(SilikoToken MyToken)
{
	switch (MyToken.Type)
	{
	case SILIKO_TOK_EOL:
		printf("EOL\n");
		break;
	case SILIKO_TOK_INTEGER:
		printf("Integer(%d)\n", MyToken.Integer);
		break;
	case SILIKO_TOK_FLOAT:
		printf("Float(%f)\n", MyToken.Float);
		break;
	case SILIKO_TOK_ID:
		printf("String(%s)\n", MyToken.Id);
		break;
	case SILIKO_TOK_ERROR:
		printf("Error");
		break;
	default:
		printf("Operator(%c)\n", (char)MyToken.Type);
	}
}


int main(int argc, char *argv[])
{
	SilikoLexer *MyLexer = SilikoLexerNew(
		SilikoStringSourceNew(
			"abs(2) + 300 / 3d6 + 5.25 * sin(200) - 5"));

	PrintToken(MyLexer->Token);
	if (MyLexer->Token.Type != SILIKO_TOK_ID
		|| strcmp(MyLexer->Token.Id, "abs") != 0)
	{
		printf("Failed, expecting: ID(abs)\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);
	if (MyLexer->Token.Type != SILIKO_TOK_LPAREN)
	{
		printf("Failed, expecting: (\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);
	if (MyLexer->Token.Type != SILIKO_TOK_INTEGER
		|| MyLexer->Token.Integer != 2)
	{
		printf("Failed, expecting: Integer: 2\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_RPAREN)
	{
		printf("Failed, expecting: )\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_ADDITION)
	{
		printf("Failed, expecting: +\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_INTEGER
		|| MyLexer->Token.Integer != 300)
	{
		printf("Failed, expecting: Integer: 300\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_DIVISION)
	{
		printf("Failed, expecting: /\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_INTEGER
		|| MyLexer->Token.Integer != 3)
	{
		printf("Failed, expecting: Integer: 3\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_DICE)
	{
		printf("Failed, expecting: d\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_INTEGER
		|| MyLexer->Token.Integer != 6)
	{
		printf("Failed, expecting: Integer: 6\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_ADDITION)
	{
		printf("Failed, expecting: +\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_FLOAT
		|| MyLexer->Token.Float != 5.25)
	{
		printf("Failed, expecting: Integer: 5.25\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	//Character(*)
	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_MULTIPLY)
	{
		printf("Failed, expecting: *\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	//ID(sin)
	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_ID
		|| strcmp(MyLexer->Token.Id, "sin") != 0)
	{
		printf("Failed, expecting: ID(sin)\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_LPAREN)
	{
		printf("Failed, expecting: (\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_INTEGER
		|| MyLexer->Token.Integer != 200)
	{
		printf("Failed, expecting: Integer: 200\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_RPAREN)
	{
		printf("Failed, expecting: )\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_SUBTRACT)
	{
		printf("Failed, expecting: -\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_INTEGER
		|| MyLexer->Token.Integer != 5)
	{
		printf("Failed, expecting: Integer: 5\n");
		return 1;
	}
	SilikoLexerNext(MyLexer);

	PrintToken(MyLexer->Token);;
	if (MyLexer->Token.Type != SILIKO_TOK_EOL)
	{
		printf("Failed, expecting: EOL\n");
		return 1;
	}

	printf("Total Success!\n");
	return 0;
}
