// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Siliko.

// Siliko is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Siliko is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Siliko. If not, see
// <http://www.gnu.org/licenses/>.


#include <ctype.h>
#include <stdlib.h>

#include <SilikoCore/Input.h>
#include <SilikoCore/Lexer.h>
#include <SilikoCore/Token.h>

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

static inline int
IsOperator(int character)
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

static inline int
IsIdCharacter(int character)
{
	return (isalnum(character) || character == '_');
}

enum SilikoDfaState
{
	DfaError = -1,
	DfaStart = 0,	/* "" */
	DfaEuler,		/* "e" */
	DfaPiStart,	    /* "p" */
	DfaPiFull,	    /* "pi" */
	DfaDice,	    /* "d" */
	DfaId,		    /* [a-ce-z][a-z0-9]* or d[a-z][a-z0-9]* */
	DfaInteger,     /* [0-9]+ */
	DfaReal,	    /* [0-9]+\.[0-9]+ */
	DfaTerminateInteger,
	DfaTerminateReal,
	DfaTerminateEuler,
	DfaTerminatePi,
	DfaTerminateCharacter,
	DfaTerminateId,
	DfaTerminateEndOfInput,
	DfaFinish
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

	SilikoDfaState dfaState = DfaStart;
	while (dfaState != DfaFinish)
	switch (dfaState)
	{
	case DfaFinish:
		break;
	case DfaStart:
		if (IsOperator(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			Append(&Lex, '\0');
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaTerminateCharacter;
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == 'd')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaDice;
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == 'e')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaEuler;
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == 'p')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaPiStart;
		}
		else if (isdigit(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaInteger;
		}
		else if (isalpha(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaId;
		}
		else if (isspace(SilikoInputGetCharacter(Lexer->Source)))
		{
			SilikoInputAdvance(Lexer->Source);
		}
		else if (SilikoInputGetCharacter(Lexer->Source) == '\0')
		{
			dfaState = DfaTerminateEndOfInput;
		}
		else
		{
			dfaState = DfaError;
		}
		break;
	case DfaDice:
		if (isalpha(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaId;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminateCharacter;
		}
		break;
	case DfaEuler:
		if (isalnum(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaId;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminateEuler;
		}
		break;
	case DfaPiStart:
		if (SilikoInputGetCharacter(Lexer->Source) == 'i')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaPiFull;
		}
		else if (IsIdCharacter(
				SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaId;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminateId;
		}
		break;
	case DfaPiFull:
		if (IsIdCharacter(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaId;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminatePi;
		}
		break;
	case DfaId:
		if (isalnum(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminateId;
		}
		break;
	case DfaInteger:
		if (SilikoInputGetCharacter(Lexer->Source) == '.')
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
			dfaState = DfaReal;
		}
		else if (isdigit(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminateInteger;
		}
		break;
	case DfaReal:
		if (isdigit(SilikoInputGetCharacter(Lexer->Source)))
		{
			Append(&Lex, SilikoInputGetCharacter(Lexer->Source));
			SilikoInputAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DfaTerminateReal;
		}
		break;
	case DfaTerminateInteger:
		SilikoTokenAssignInteger(
			Lexer->Token, strtoll(Lex.Buffer, NULL, 10));
		dfaState = DfaFinish;
		break;
	case DfaTerminateReal:
		SilikoTokenAssignReal(Lexer->Token, atof(Lex.Buffer));
		dfaState = DfaFinish;
		break;
	case DfaTerminateEuler:
		SilikoTokenAssignReal(Lexer->Token, EULER);
		dfaState = DfaFinish;
		break;
	case DfaTerminatePi:
		SilikoTokenAssignReal(Lexer->Token, PI);
		dfaState = DfaFinish;
		break;
	case DfaTerminateCharacter:
		SilikoTokenAssignCharacter(Lexer->Token, Lex.Buffer[0]);
		dfaState = DfaFinish;
		break;
	case DfaTerminateId:
		SilikoTokenAssignId(Lexer->Token, Lex.Buffer);
		dfaState = DfaFinish;
		break;
	case DfaTerminateEndOfInput:
		SilikoTokenAssignEndOfInput(Lexer->Token);
		dfaState = DfaFinish;
		break;
	case DfaError:
		Lexer->error = -1;
		dfaState = DfaFinish;
		break;
	}
	free(Lex.Buffer);
}

SilikoLexer *SilikoLexerCreate(SilikoInput *source)
{
	SilikoLexer *object = malloc(sizeof(SilikoLexer));
	if (!object)
		return NULL;
	SilikoToken *new_token = SilikoTokenCreate();
	if (!new_token)
	{
		free(object);
		return NULL;
	}

	object->Source = source;
	object->Token = new_token;
	object->error = 0;
	SilikoLexerAdvance(object);

	return object;
}

void SilikoLexerDestroy(SilikoLexer *Lexer)
{
	if (Lexer)
	{
		SilikoInputDelete(Lexer->Source);
		SilikoTokenDestroy(Lexer->Token);
		free(Lexer);
	}
}

const SilikoToken *SilikoLexerGetToken(SilikoLexer *Lexer)
{
	return Lexer->Token;
}
