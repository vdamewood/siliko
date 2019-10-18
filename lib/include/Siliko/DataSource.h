/* DataSource.h: Abstract interface for input data
 * Copyright 2015, 2016 Vincent Damewood
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

#if !defined SILIKO_DATA_SOURCE
#define SILIKO_DATA_SOURCE

#include <Siliko/W32Dll.h>

#if defined __cplusplus
extern "C" {
#endif

struct SILIKO_API SilikoDataSource
{
	void *State;
	int (*AdvanceFunction)(void *State);
	char (*GetFunction)(void *State);
	void (*DeleteFunction)(void *State);
};
typedef struct SilikoDataSource SilikoDataSource;

SILIKO_API int SilikoDataSourceAdvance(SilikoDataSource *Source);
SILIKO_API char SilikoDataSourceGet(SilikoDataSource *Source);
SILIKO_API void SilikoDataSourceDelete(SilikoDataSource *Source);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_DATA_SOURCE */
