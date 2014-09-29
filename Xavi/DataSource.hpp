/*
 * XaviDataSource.hpp: Abstract interface for input data
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

#if !defined DATASOURCE_HPP
#define DATASOURCE_HPP

namespace Xavi
{
	class DataSource
	{
	public:
		virtual ~DataSource() { };
		virtual bool Advance() = 0;
		virtual char GetCurrent() = 0;
	};
};

#endif /* DATASOURCE_HPP */
