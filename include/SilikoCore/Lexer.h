/* Lexer.h: Lexical analyzer
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

#if !defined SILIKO_CORE_LEXER_H
#define SILIKO_CORE_LEXER_H

#include <SilikoCore/Api.h>
#include <SilikoCore/DataSource.h>
#include <SilikoCore/Token.h>

#if defined __cplusplus
extern "C" {
#endif

struct SilikoLexer;
typedef struct SilikoLexer SilikoLexer;

SILIKOCORE_EXPORT
    SilikoLexer *SilikoLexerNew(SilikoDataSource *input);
SILIKOCORE_EXPORT
    void SilikoLexerDelete(SilikoLexer*);

SILIKOCORE_EXPORT
    void SilikoLexerAdvance(SilikoLexer*);
SILIKOCORE_EXPORT
    const SilikoToken *SilikoLexerGetCurrent(SilikoLexer*);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_LEXER_H
