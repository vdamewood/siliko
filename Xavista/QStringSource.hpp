/*
 *  QStringSource.hpp: Input Source abstaction for a QString
 *  Copyright 2014 Vincent Damewood
 *
 *  This file is part of Xavista.
 *
 *  Xavista is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Xavista is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Xavista. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined Q_STRING_SOURCE_H
#define Q_STRING_SOURCE_H

#include <QString>
#include <Xavi/DataSource.hpp>

class QStringSource : public Xavi::DataSource
{
public:
	QStringSource(const QString &);
	virtual bool Advance();
	virtual char GetCurrent();
	virtual ~QStringSource();

private:
	QString Source;
	QString::iterator Iterator;
};

#endif /* Q_STRING_SOURCE_H */
