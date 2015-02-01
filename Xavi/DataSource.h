/*
 * DataSource.h: Abstract interface for input data
 * Copyright 2015 Vincent Damewood
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

#if !defined XAVI_DATA_SOURCE
#define XAVI_DATA_SOURCE

#include "W32Dll.h"

#if defined __cplusplus
extern "C" {
#endif

struct XAVI_API XaviDataSource
{
	void *State;
	int (*AdvanceFunction)(void*);
	char (*GetFunction)(void*);
	void (*DestroyFunction)(void*);
};
typedef struct XaviDataSource XaviDataSource;

XAVI_API int XaviDataSourceAdvance(XaviDataSource *);
XAVI_API char XaviDataSourceGet(XaviDataSource *);
XAVI_API void XaviDataSourceDestroy(XaviDataSource *);

#if defined __cplusplus
}
#endif

#endif /* XAVI_DATA_SOURCE */
