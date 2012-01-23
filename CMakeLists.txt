cmake_minimum_required (VERSION 2.6)
SET(CMAKE_BUILD_TYPE Release)
project(XVCalc)

FIND_PACKAGE(Qt4)

IF (UNIX AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(MATH_LIB "m")
ELSE(UNIX AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(MATH_LIB )
ENDIF(UNIX AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_subdirectory("libxvcalc")
add_subdirectory("tests")

IF(QT4_FOUND)
	MESSAGE("Qt4 Was found. Configuring XvcGui.")
	add_subdirectory("XvcGui")
ELSE(QT4_FOUND)
	MESSAGE("Qt4 was not found. Skipping XvcGui.")
ENDIF(QT4_FOUND)
