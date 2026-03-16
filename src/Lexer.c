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
	SilikoInput *input;
	SilikoToken *token;
	int error;
	bool supportDice;
	bool supportConstants;
};

// Taken from
// The Art of Computer Programming, Volume 2, Third Edition
// By Donald E. Knuth
static const double Pi =    0x3.243f6a8885a30p0;
static const double Euler = 0x2.b7e151628aed2p0;

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

struct Lexeme
{
	char *buffer;
	size_t current;
	size_t end;
};

static int Append(struct Lexeme *object, char new_character)
{
	if (object->current == object->end)
	{
		size_t new_end = object->end * 2;
		char *new_buffer = realloc(object->buffer, new_end);
		if (!new_buffer)
		{
			return 0;
		}
		else
		{
			object->buffer = new_buffer;
			object->end = new_end;
		}
	}

	object->buffer[object->current++] = new_character;
	return -1;
}

SilikoLexer *SilikoLexerCreate(
	SilikoInput *source_input,
	bool support_dice,
	bool support_constants)
{
	SilikoLexer *object = malloc(sizeof *object);
	if (!object)
		return NULL;

	SilikoToken *new_token = SilikoTokenCreate();
	if (!new_token)
	{
		free(object);
		return NULL;
	}

	object->input = source_input;
	object->token = new_token;
	object->error = 0;
	object->supportDice = support_dice;
	object->supportConstants = support_constants;
	SilikoLexerAdvance(object);

	return object;
}

void SilikoLexerDestroy(SilikoLexer *object)
{
	if (object)
	{
		SilikoInputDestroy(object->input);
		SilikoTokenDestroy(object->token);
		free(object);
	}
}

void SilikoLexerAdvance(SilikoLexer *object)
{
	if (!object)
		return;

	if (SilikoTokenGetStatus(object->token) == SilikoTokenEndOfInput
			|| object->error)
		return;

	struct Lexeme lexeme = {malloc(4), 0, 4};
	if (!(lexeme.buffer))
	{
		object->error = -1;
		return;
	}

	enum SilikoDfaState dfa_state = DfaStart;
	while (dfa_state != DfaFinish)
	switch (dfa_state)
	{
	case DfaFinish:
		break;
	case DfaStart:
		if (IsOperator(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			Append(&lexeme, '\0');
			SilikoInputAdvance(object->input);
			dfa_state = DfaTerminateCharacter;
		}
		else if (object->supportDice
			&& SilikoInputGetCharacter(object->input) == 'd')
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaDice;
		}
		else if (object->supportConstants
			&& SilikoInputGetCharacter(object->input) == 'e')
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaEuler;
		}
		else if (object->supportConstants
			&& SilikoInputGetCharacter(object->input) == 'p')
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaPiStart;
		}
		else if (isdigit(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaInteger;
		}
		else if (isalpha(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaId;
		}
		else if (isspace(SilikoInputGetCharacter(object->input)))
		{
			SilikoInputAdvance(object->input);
		}
		else if (SilikoInputGetCharacter(object->input) == '\0')
		{
			dfa_state = DfaTerminateEndOfInput;
		}
		else
		{
			dfa_state = DfaError;
		}
		break;
	case DfaDice:
		if (isalpha(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaId;
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminateCharacter;
		}
		break;
	case DfaEuler:
		if (isalnum(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaId;
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminateEuler;
		}
		break;
	case DfaPiStart:
		if (SilikoInputGetCharacter(object->input) == 'i')
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaPiFull;
		}
		else if (IsIdCharacter(
				SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaId;
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminateId;
		}
		break;
	case DfaPiFull:
		if (IsIdCharacter(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaId;
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminatePi;
		}
		break;
	case DfaId:
		if (isalnum(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminateId;
		}
		break;
	case DfaInteger:
		if (SilikoInputGetCharacter(object->input) == '.')
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
			dfa_state = DfaReal;
		}
		else if (isdigit(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminateInteger;
		}
		break;
	case DfaReal:
		if (isdigit(SilikoInputGetCharacter(object->input)))
		{
			Append(&lexeme, SilikoInputGetCharacter(object->input));
			SilikoInputAdvance(object->input);
		}
		else
		{
			Append(&lexeme, '\0');
			dfa_state = DfaTerminateReal;
		}
		break;
	case DfaTerminateInteger:
		SilikoTokenAssignFromInteger(
			object->token, strtoll(lexeme.buffer, NULL, 10));
		dfa_state = DfaFinish;
		break;
	case DfaTerminateReal:
		SilikoTokenAssignFromReal(object->token,
			strtof(lexeme.buffer, NULL));
		dfa_state = DfaFinish;
		break;
	case DfaTerminateEuler:
		SilikoTokenAssignFromReal(object->token, Euler);
		dfa_state = DfaFinish;
		break;
	case DfaTerminatePi:
		SilikoTokenAssignFromReal(object->token, Pi);
		dfa_state = DfaFinish;
		break;
	case DfaTerminateCharacter:
		SilikoTokenAssignFromCharacter(object->token, lexeme.buffer[0]);
		dfa_state = DfaFinish;
		break;
	case DfaTerminateId:
		SilikoTokenAssignFromId(object->token, lexeme.buffer);
		dfa_state = DfaFinish;
		break;
	case DfaTerminateEndOfInput:
		SilikoTokenAssignEndOfInput(object->token);
		dfa_state = DfaFinish;
		break;
	case DfaError:
		object->error = -1;
		dfa_state = DfaFinish;
		break;
	}
	free(lexeme.buffer);
}

const SilikoToken *SilikoLexerGetToken(SilikoLexer *object)
{
	if (!object)
		return NULL;

	return object->token;
}
