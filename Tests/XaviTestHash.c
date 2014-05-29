/*
 * XaviTestHash.c: Program to test Xavi's hash function.
 * Copyright 2014 Vincent Damewood
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

#include <stdio.h>
#include <string.h>

unsigned char XaviHash(const unsigned char *rawInput, size_t length)
;
int main(int argc, char * argv[])
{
	if (argc == 2)
	{
		int len = strlen(argv[1]);
		int i = 0;
		for (; i <= len-1; i++)
			printf("%02X", argv[1][i]);
		printf("\n");
		printf("%d\n", (int) XaviHash(argv[1], len));
	}

	return 0;
}
