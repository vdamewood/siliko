/*
 * CStringSource.hpp: Class to input data from a C-style string
 * Copyright 2014, 2015 Vincent Damewood
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

#if !defined XAVIPP_CSTRING_SOURCE_HPP
#define XAVIPP_CSTRING_SOURCE_HPP

#include "W32Dll.hpp"
#include "DataSource.hpp"

namespace Xavi
{
	class XAVIPP_API CStringSource : public DataSource
	{
	public:
		CStringSource(const char *);
		virtual bool Advance();
		virtual char GetCurrent();
		virtual ~CStringSource();

	private:
		char *Source;
		int Index;
	};
};

#endif // XAVIPP_CSTRING_SOURCE_HPP
