/*
 * Lexer.cpp: Lexical analyzer
 * Copyright 2012, 2014, 2015 Vincent Damewood
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

void Xavi::Lexer::Next(void)
{
	XaviDfaState dfaState = DFA_START;
	std::string lexeme = std::string();

	if (MyToken->Type() == Token::EOL || MyToken->Type() == Token::ERROR)
		return;

	delete MyToken;

	while (dfaState != DFA_END)
	switch (dfaState)
	{
	case DFA_END:
		break;
	case DFA_START:
		if(isOperator(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_TERM_CHAR;
		}
		else if (MySource->GetCurrent() == 'd')
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_DICE;
		}
		else if (MySource->GetCurrent() == 'e')
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_E;
		}
		else if (MySource->GetCurrent() == 'p')
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_PI_1;
		}
		else if (std::isdigit(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_INTEGER;
		}
		else if (std::isalpha(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_ID;
		}
		else if (std::isspace(MySource->GetCurrent()))
		{
			MySource->Advance();
		}
		else if (MySource->GetCurrent() == '\0')
		{
			dfaState = DFA_TERM_EOI;
		}
		else
		{
			dfaState = DFA_ERROR;
		}
		break;
	case DFA_DICE:
		if (std::isalpha(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_CHAR;
		}
		break;
	case DFA_E:
		if (std::isalnum(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_E;
		}
		break;
	case DFA_PI_1:
		if (MySource->GetCurrent() == 'i')
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_PI_2;
		}
		else if (isIdCharacter(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_PI_2:
		if (isIdCharacter(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_PI;
		}
		break;
	case DFA_ID:
		if (isalnum(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
		}
		else
		{
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_INTEGER:
		if (MySource->GetCurrent() == '.')
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
			dfaState = DFA_FLOAT;
		}
		else if (std::isdigit(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
		}
		else
		{
			dfaState = DFA_TERM_INTEGER;
		}
		break;
	case DFA_FLOAT:
		if (std::isdigit(MySource->GetCurrent()))
		{
			lexeme += MySource->GetCurrent();
			MySource->Advance();
		}
		else
		{
			dfaState = DFA_TERM_FLOAT;
		}
		break;
	case DFA_TERM_INTEGER:
		MyToken = new Token(atoi(lexeme.c_str()));
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		MyToken = new Token(static_cast<float>(atof(lexeme.c_str())));
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		MyToken = new Token(static_cast<float>(EULER));
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		MyToken = new Token(static_cast<float>(PI));
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		MyToken = new Token(static_cast<Token::TokenType>(lexeme[0]));
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		MyToken = new Token(lexeme.c_str());
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOI:
		MyToken = new Token(Token::EOL);
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		MyToken = new Token(Token::ERROR);
		dfaState = DFA_END;
		break;
	}
}

Xavi::Lexer::Lexer(DataSource * InputSource)
{
	MySource = InputSource;
	MyToken = new Token(Token::UNSET);
	Next();
}

Xavi::Lexer::~Lexer(void)
{
	delete MyToken;
	delete MySource;
}

Xavi::Token &Xavi::Lexer::GetToken(void)
{
	return *MyToken;
}
