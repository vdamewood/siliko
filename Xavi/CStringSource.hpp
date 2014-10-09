/*
 * StringSource.hpp: Class to input data from a string.
 * Copyright 2014 Vincent Damewood
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

#if !defined XAVI_STRING_SOURCE_HPP
#define XAVI_STRING_SOURCE_HPP

#include <string>

#include "W32Dll.hpp"
#include "DataSource.hpp"

namespace Xavi
{
	class API StringSource : public DataSource
	{
	public:
		StringSource(const char *);
		virtual bool Advance();
		virtual char GetCurrent();
		virtual ~StringSource();

	private:
		char *Source;
		int Index;
	};
};

#endif // XAVI_STRIN_GSOURCE_HPP
