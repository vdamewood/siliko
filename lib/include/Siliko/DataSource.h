/* DataSource.h: Abstract interface for input data
 * Copyright 2012-2021 Vincent Damewood
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

#include <Siliko/Api.h>

#if defined __cplusplus
extern "C" {
#endif

struct SilikoDataSource
{
	void *State;
	int (*AdvanceFunction)(void *State);
	char (*GetFunction)(void *State);
	void (*DeleteFunction)(void *State);
};
typedef struct SilikoDataSource SilikoDataSource;

SILIKO_EXPORT int SilikoDataSourceAdvance(SilikoDataSource *Source);
SILIKO_EXPORT char SilikoDataSourceGet(SilikoDataSource *Source);
SILIKO_EXPORT void SilikoDataSourceDelete(SilikoDataSource *Source);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_DATA_SOURCE */
