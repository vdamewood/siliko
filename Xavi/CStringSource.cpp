/*
 * XaviCStringSource.cpp: Class for input data from a C-style string
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

#include "CStringSource.hpp"

Xavi::CStringSource::CStringSource(const char * NewSource)
{
	Source = NewSource;
	Index = 0;
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

Xavi::CStringSource::~CStringSource()
{
	// Do Nothing
}
