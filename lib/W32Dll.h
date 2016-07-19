/* W32Dll.h: Header to accomodate building a DLL on Microsoft Windows
 * Copyright 2014, 2015, 2016 Vincent Damewood
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

#if !defined SILIKO_W32_DLL

#if defined _WIN32 && defined _MSC_VER && !defined SILIKO_STATIC
#  if defined SilikoDLib_EXPORTS
#    define SILIKO_API __declspec(dllexport)
#    define SILIKO_EXTERN
#  else // SILIKO_EXPORTS
#    define SILIKO_API __declspec(dllimport)
#    define SILIKO_EXTERN extern
#  endif // SILIKO_EXPORTS
#else // _WIN32 && __MSC_VER
#  define SILIKO_API
#  define SILIKO_EXTERN
#endif // _WIN32 && __MSC_VER

#endif // SILIKO_W32_DLL
