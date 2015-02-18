/*
 * StringSource.hpp: Class to input data from a string
 * Copyright 2014, 2015 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined STRING_SOURCE_HPP
#define STRING_SOURCE_HPP

#include <string>
#include <XaviPP/DataSource.hpp>

class StringSource : public Xavi::DataSource
{
public:
	StringSource(const std::string &);
	virtual bool Advance();
	virtual char GetCurrent();
	virtual ~StringSource();

private:
	const std::string Source;
	std::string::const_iterator Iterator;;
};

#endif // STRING_SOURCE_HPP
