/*
 * XaviFunctionCall.h: Function handling.
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

#if !defined Xavi_FUNCTIONS_H
#define Xavi_FUNCTIONS_H

#include <string>
#include <vector>

#include "XaviValue.hpp"

namespace Xavi
{
	namespace Functions
	{
		int Open();
		void Close();
		XaviValue Call(std::string FunctionName, std::vector<XaviValue> Arguments);
	};
};
#endif // Xavi_FUNCTIONS_H
