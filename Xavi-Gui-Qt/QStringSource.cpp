/*
 * QStringSource.cpp: Input Source abstaction for a QString
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

#include <cstdlib>

#include "QStringSource.hpp"

struct QStringSourceState
{
	QString string;
	QString::iterator iterator;

	QStringSourceState(const QString &newString);
};

QStringSourceState::QStringSourceState(const QString &newString)
	: string(QString(newString)),
	iterator(string.begin())
{
}


extern "C"
{
	static int QStringSourceAdvance(void *vState);
	static char QStringSourceGet(void *vState);
	static void QStringSourceDelete(void *State);
};

static int QStringSourceAdvance(void *vState)
{
	QStringSourceState *State = (QStringSourceState *)vState;

	if (State->iterator != State->string.end())
	{
		State->iterator++;
		return -1;
	}
	else
	{
		return 0;
	}
}

static char QStringSourceGet(void *vState)
{
	QStringSourceState *State = (QStringSourceState *)vState;

	if (State->iterator != State->string.end())
	{
		return State->iterator->toLatin1();
	}
	else
	{
		return '\0';
	}
}

static void QStringSourceDelete(void *vState)
{
	delete (QStringSourceState*)vState;
}


XaviDataSource *QStringSourceNew(const QString &NewSource)
{
	QStringSourceState *rValState;
	XaviDataSource *rVal;
	if (!(rVal = (XaviDataSource *) std::malloc(sizeof(XaviDataSource))))
		throw std::bad_alloc();
	rValState = new QStringSourceState(NewSource);

	rVal->State = (void *)rValState;
	rVal->AdvanceFunction = QStringSourceAdvance;
	rVal->GetFunction = QStringSourceGet;
	rVal->DeleteFunction = QStringSourceDelete;
	return rVal;
}
