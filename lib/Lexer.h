/*
 * Lexer.h: Lexical analyzer
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

#if !defined SILIKO_LEXER_H
#define SILIKO_LEXER_H

#include "W32Dll.h"
#include "DataSource.h"
#include "Token.h"

#if defined __cplusplus
extern "C" {
#endif

struct SILIKO_API SilikoLexer
{
	SilikoDataSource *Source;
	SilikoToken Token;
};
typedef struct SilikoLexer SilikoLexer;

SILIKO_API SilikoLexer *SilikoLexerNew(SilikoDataSource *InputSource);
SILIKO_API void SilikoLexerDelete(SilikoLexer *Lexer);

SILIKO_API void SilikoLexerNext(SilikoLexer *Lexer);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_LEXER_H */
