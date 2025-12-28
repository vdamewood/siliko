/* Lexer.c: Lexical analyzer
 * Copyright 2012-2025 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <SilikoCore/Input.h>
#include <SilikoCore/Lexer.h>

struct SilikoLexer
{
	SilikoInput *Source;
	SilikoToken *Token;
	int error;
};
typedef struct SilikoLexer SilikoLexer;


/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

static int isOperator(int character)
{
	return
	(
		character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '*'
		|| character == '^'
		|| character == ','
		|| character == '('
		|| character == ')'
	);
}

static int isIdCharacter(int character)
{
	return (isalnum(character) || character == '_');
}

enum SilikoDfaState
{
	DFA_ERROR = -1,
	DFA_START = 0,	/* "" */
	DFA_E,		/* "e" */
	DFA_PI_1,	/* "p" */
	DFA_PI_2,	/* "pi" */
	DFA_DICE,	/* "d" */
	DFA_ID,		/* ([a-ce-z][a-z0-9]*) | (d[a-z][a-z0-9]*) */
	DFA_INTEGER,	/* [0-9]+ */
	DFA_FLOAT,	/* [0-9]+\.[0-9]+ */
	DFA_TERM_INTEGER,
	DFA_TERM_FLOAT,
	DFA_TERM_E,
	DFA_TERM_PI,
	DFA_TERM_CHAR,
	DFA_TERM_STRING,
	DFA_TERM_EOL,
	DFA_END
};
typedef enum SilikoDfaState SilikoDfaState;

struct Lexeme
{
	char *Buffer;
	size_t Current;
	size_t End;
};
typedef struct Lexeme Lexeme;

static int Append(Lexeme *Lex, char NewChar)
{
	if (Lex->Current == Lex->End)
	{
		char *Temp;
		Lex->End++;
		if (!(Temp = realloc(Lex->Buffer, Lex->End)))
			return 0;
		else
			Lex->Buffer = Temp;
	}

	Lex->Buffer[Lex->Current++] = NewChar;
	return -1;
}

void SilikoLexerAdvance(SilikoLexer *Lexer)
{
	if (SilikoTokenGetStatus(Lexer->Token) == SilikoTokenEndOfInput
			|| Lexer->error)
		return;

	Lexeme Lex = {malloc(4), 0, 4};
	if (!(Lex.Buffer))
	{
		Lexer->error = -1;
		return;
	}

	SilikoDfaState dfaState = DFA_START;
	while (dfaState != DFA_END)
	switch (dfaState)
	{
	case DFA_END:
		break;
	case DFA_START:
		if (isOperator(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			Append(&Lex, '\0');
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_TERM_CHAR;
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == 'd')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_DICE;
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == 'e')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_E;
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == 'p')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_PI_1;
		}
		else if (isdigit(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_INTEGER;
		}
		else if (isalpha(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else if (isspace(SilikoInputGetCharacter(Lexer->Source)))
		{
			SilikoInputAdvance(Lexer->Source);
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == '\0')
		{
			dfaState = DFA_TERM_EOL;
		}
		else
		{
			dfaState = DFA_ERROR;
		}
		break;
	case DFA_DICE:
		if (isalpha(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_CHAR;
		}
		break;
	case DFA_E:
		if (isalnum(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_E;
		}
		break;
	case DFA_PI_1:
		if (SilikoInputGetCharacter(Lexer->Source) == 'i')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_PI_2;
		}
		else if (isIdCharacter(
				SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_PI_2:
		if (isIdCharacter(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_PI;
		}
		break;
	case DFA_ID:
		if (isalnum(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_INTEGER:
		if (SilikoInputGetCharacter(Lexer->Source) == '.')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DFA_FLOAT;
		}
		else if (isdigit(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_INTEGER;
		}
		break;
	case DFA_FLOAT:
		if (isdigit(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_FLOAT;
		}
		break;
	case DFA_TERM_INTEGER:
		SilikoTokenAssignInteger(Lexer->Token, strtoll(Lex.Buffer, NULL, 10));
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		SilikoTokenAssignReal(Lexer->Token, atof(Lex.Buffer));
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		SilikoTokenAssignReal(Lexer->Token, EULER);
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		SilikoTokenAssignReal(Lexer->Token, PI);
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		SilikoTokenAssignCharacter(Lexer->Token, Lex.Buffer[0]);
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		SilikoTokenAssignId(Lexer->Token, Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOL:
		SilikoTokenAssignEndOfInput(Lexer->Token);
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		Lexer->error = -1;
		dfaState = DFA_END;
		break;
	}
	free(Lex.Buffer);
}

SilikoLexer *SilikoLexerNew(SilikoInput *InputSource)
{
	SilikoLexer *rVal = malloc(sizeof(SilikoLexer));

	if (!(rVal))
		return NULL;

	rVal->Source = InputSource;
	rVal->Token = SilikoTokenNew();
	rVal->error = 0;
	SilikoLexerAdvance(rVal);

	return rVal;
}

void SilikoLexerDelete(SilikoLexer *Lexer)
{
	if (Lexer)
	{
		SilikoInputDelete(Lexer->Source);
		SilikoTokenDelete(Lexer->Token);
		free(Lexer);
	}
}

const SilikoToken *SilikoLexerGetCurrent(SilikoLexer *Lexer)
{
	return Lexer->Token;
}
