/*
 * W32Dll.h: Header to accomodate building a DLL on Microsoft Windows
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

#if !defined XAVI_W32_DLL

#if defined _WIN32 && defined _MSC_VER
#  if defined Xavi_EXPORTS
#    define XAVI_API __declspec(dllexport)
#    define XAVI_EXTERN
#  else // Xavi_EXPORTS
#    define XAVI_API __declspec(dllimport)
#    define XAVI_EXTERN extern
#  endif // Xavi_EXPORTS
#else // _WIN32 && __MSC_VER
#  define XAVI_API
#  define XAVI_EXTERN
#endif // _WIN32 && __MSC_VER

#endif // XAVI_W32_DLL
