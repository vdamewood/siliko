# SilikoConfig.cmake: Find Siliko library
# Copyright 2016, 2017 Vincent Damewood
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

include(FindPackageHandleStandardArgs)

find_path(Siliko_INC
  NAMES Siliko/Value.h
  DOC "Siliko Include Files")

find_library(Siliko_LIB
  NAMES Siliko
  DOC "Siliko Library")

find_package_handle_standard_args(Siliko DEFAULT_MSG Siliko_INC Siliko_LIB)

if(Siliko_FOUND)
  # Detect if system has separate math library (libm)
  set(LM)
  include(CheckSymbolExists)
  check_symbol_exists(sin math.h FOUND_SIN)
  if(NOT "${FOUND_SIN}")
    set(CMAKE_REQUIRED_LIBRARIES m)
    check_symbol_exists(sin math.h FOUND_SIN_WITH_LM)
    if(${FOUND_SIN_WITH_LM})
      set(LM m)
    endif()
  endif()

  set(Siliko_LIBRARIES    ${Siliko_LIB} ${LM})
  set(Siliko_INCLUDE_DIRS ${Siliko_INC})
  set(Siliko_DEFINITIONS  )
endif()

mark_as_advanced(Siliko_INC Siliko_LIB)
