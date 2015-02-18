/*
 * DataSource.hpp: Abstract interface for input data
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

#if !defined XAVI_DATA_SOURCE_HPP
#define XAVI_DATA_SOURCE_HPP

#include "W32Dll.hpp"

namespace Xavi
{
	class XAVIPP_API DataSource
	{
	public:
		virtual ~DataSource() { };
		virtual bool Advance() = 0;
		virtual char GetCurrent() = 0;
	};
};

#endif // XAVIPP_DATA_SOURCE_HPP
