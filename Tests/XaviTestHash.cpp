/*
 * XaviTestHash.cpp: Program to test Xavi's hash function.
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

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <Xavi++/Hash.hpp>

int main(int argc, char * argv[])
{
	std::vector<std::string> Tests;
	std::vector<unsigned char> Expected;
	Tests.push_back("abs");
	Expected.push_back(0x08);

	Tests.push_back("addition");
	Expected.push_back(0xAF);

	Tests.push_back(" ");
	Expected.push_back(0xA4);

	Tests.push_back("qwert");
	Expected.push_back(0x11);

	Tests.push_back("dice");
	Expected.push_back(0xB9);

	Tests.push_back("sin");
	Expected.push_back(0xD7);

	int Success = 0;
	unsigned char Result = 0x0;

	for (int i = 0; i < Tests.size(); i++)
	{
		std::cout << "Attempting " << Tests[i] << ": ";
		Result = Xavi::Crc8((unsigned char *)Tests[i].c_str(), strlen(Tests[i].c_str()));
		if (Result == Expected[i])
		{
			std::cout << "Success" << std::endl;
			Success++;
		}
		else
		{
			std::cout << "failed. Expected: " << Expected[i]
				<< "; Got: " << Result << ";" << std::endl;
		}
	}

	if (Success == Tests.size())
		std::cout << "Total success." << std::endl;

	return Tests.size() - Success;
}
