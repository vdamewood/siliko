/*
 * Lexer.cpp: Lexical analyzer.
 * Copyright 2012, 2014 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <cctype>
#include <cstring>

#include "Lexer.hpp"

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
	return (std::isalnum(character) || character == '_');
}

enum XaviDfaState
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
	DFA_TERM_EOI,
	DFA_END
};

void Xavi::Lexer::Load(void)
{
	XaviDfaState dfaState = DFA_START;
	std::string lexeme = std::string();

	if (token->GetType() == Token::EOL || token->GetType() == Token::ERROR)
		return;

	delete token;

	while (dfaState != DFA_END)
	switch (dfaState)
	{
	case DFA_END:
		break;
	case DFA_START:
		if(isOperator(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_TERM_CHAR;
		}
		else if (Source->GetCurrent() == 'd')
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_DICE;
		}
		else if (Source->GetCurrent() == 'e')
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_E;
		}
		else if (Source->GetCurrent() == 'p')
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_PI_1;
		}
		else if (std::isdigit(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_INTEGER;
		}
		else if (std::isalpha(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_ID;
		}
		else if (std::isspace(Source->GetCurrent()))
		{
			Source->Advance();
		}
		else if (Source->GetCurrent() == '\0')
		{
			dfaState = DFA_TERM_EOI;
		}
		else
		{
			dfaState = DFA_ERROR;
		}
		break;
	case DFA_DICE:
		if (std::isalpha(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_CHAR;
		}
		break;
	case DFA_E:
		if (std::isalnum(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_E;
		}
		break;
	case DFA_PI_1:
		if (Source->GetCurrent() == 'i')
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_PI_2;
		}
		else if (isIdCharacter(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_PI_2:
		if (isIdCharacter(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_PI;
		}
		break;
	case DFA_ID:
		if (isalnum(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
		}
		else
		{
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_INTEGER:
		if (Source->GetCurrent() == '.')
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
			dfaState = DFA_FLOAT;
		}
		else if (std::isdigit(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
		}
		else
		{
			dfaState = DFA_TERM_INTEGER;
		}
		break;
	case DFA_FLOAT:
		if (std::isdigit(Source->GetCurrent()))
		{
			lexeme += Source->GetCurrent();
			Source->Advance();
		}
		else
		{
			dfaState = DFA_TERM_FLOAT;
		}
		break;
	case DFA_TERM_INTEGER:
		token = new Token(atoi(lexeme.c_str()));
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		token = new Token(static_cast<float>(atof(lexeme.c_str())));
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		token = new Token(static_cast<float>(EULER));
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		token = new Token(static_cast<float>(PI));
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		token = new Token(static_cast<Token::Type>(lexeme[0]));
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		token = new Token(lexeme.c_str());
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOI:
		token = new Token(Token::EOL);
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		token = new Token(Token::ERROR);
		dfaState = DFA_END;
		break;
	}
}

Xavi::Lexer::Lexer(DataSource * InputSource)
{
	Source = InputSource;
	token = new Token(Token::UNSET);
	Next();
}

Xavi::Lexer::~Lexer(void)
{
	delete token;
	delete Source;
}

Xavi::Token &Xavi::Lexer::GetToken(void)
{
	return *token;
}

void Xavi::Lexer::Next(void)
{
	Load();
}
