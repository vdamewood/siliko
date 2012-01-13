cmake_minimum_required (VERSION 2.6)
project(XVCalc)

IF (UNIX AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(MATH_LIB "m")
ELSE(UNIX)
    SET(MATH_LIB )
ENDIF(UNIX AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_subdirectory("libxvcalc")
add_subdirectory("tests")
