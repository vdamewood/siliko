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


#if !defined SILIKO_CORE_STRING_INPUT_H
#define SILIKO_CORE_STRING_INPUT_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Input.h>

#if defined __cplusplus
extern "C" {
#endif

SILIKOCORE_EXPORT
SilikoInput *SilikoStringInputNew(const char *input_string);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_STRING_INPUT_H
