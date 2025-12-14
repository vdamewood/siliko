/* Value.h: Data structure to represent a single value
 * Copyright 2012-2025 Vincent Damewood
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

#if !defined SILIKO_CORE_VALUE_H
#define SILIKO_CORE_VALUE_H

#include <SilikoCore/Api.h>

#include <stdlib.h>

#if defined __cplusplus
extern "C" {
#endif

enum SilikoError
{
	SilikoErrorNone,
	SilikoErrorMemory,
	SilikoErrorSyntax,
	SilikoErrorZeroDivision,
	SilikoErrorFunctionName,
	SilikoErrorFunctionArguments,
	SilikoErrorDomain,
	SilikoErrorRange
};

enum SilikoValueStatus
{
	SilikoValueError,
	SilikoValueInteger,
	SilikoValueReal
};

struct SilikoValue;
typedef struct SilikoValue SilikoValue;

SILIKOCORE_EXPORT
	SilikoValue *SilikoValueNewError(enum SilikoError source);
SILIKOCORE_EXPORT
	SilikoValue *SilikoValueNewInteger(long long int source);
SILIKOCORE_EXPORT
	SilikoValue *SilikoValueNewReal(double source);
SILIKOCORE_EXPORT
	SilikoValue *SilikoValueNewCopy(SilikoValue *source);
SILIKOCORE_EXPORT
	void SilikoValueDelete(SilikoValue *object);

SILIKOCORE_EXPORT
	enum SilikoError SilikoValueToError(const SilikoValue *object);
SILIKOCORE_EXPORT
	long long int SilikoValueToInteger(const SilikoValue *object);
SILIKOCORE_EXPORT
	double SilikoValueToReal(const SilikoValue *object);

SILIKOCORE_EXPORT
	void SilikoValueAssignError(
		SilikoValue *object,
		enum SilikoError source);
SILIKOCORE_EXPORT
	void SilikoValueAssignInteger(
		SilikoValue *object,
		long long int source);
SILIKOCORE_EXPORT
	void SilikoValueAssignReal(
		SilikoValue *object,
		double source);
SILIKOCORE_EXPORT
	void SilikoValueAssignCopy(
		SilikoValue *object,
		const SilikoValue *source);

SILIKOCORE_EXPORT
	enum SilikoValueStatus SilikoValueGetStatus(
		const SilikoValue *object);
SILIKOCORE_EXPORT
	void SilikoValueNegate(SilikoValue *object);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_VALUE_H
