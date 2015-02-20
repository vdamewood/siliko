/*
 * CStringSource.cpp: Class to input data from a C-style string
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

// Visual Studio complains about strcpy without this.
#if defined _WIN32 && defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _WIN32 && defined _MSC_VER */

#include <cstring>

#include "CStringSource.hpp"

Xavi::CStringSource::CStringSource(const char *NewSource)
{
	Source = new char[std::strlen(NewSource) + 1];
	std::strcpy(Source, NewSource);
	Index = 0;
}

Xavi::CStringSource::~CStringSource()
{
	delete[] Source;
}

bool Xavi::CStringSource::Advance()
{
	if (Source[Index] != '\0')
	{
		Index++;
		return true;
	}
	else
	{
		return false;
	}
}

char Xavi::CStringSource::GetCurrent()
{
	return Source[Index];
}
