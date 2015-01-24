# Root

# Semi-Required Preamble
cmake_minimum_required(VERSION 2.8.11)
project(Xavi)
set(Xavi_VERSION_MAJOR 0)
set(Xavi_VERSION_MINOR 0)
set(Xavi_VERSION_PATCH 0)


# Dependency searches
message("-- Finding Qt5Widgets")
find_package(Qt5Widgets QUIET)

if(Qt5Widgets_FOUND)
  message("-- Finding Qt5Widgets - found")
else()
  message("-- Finding Qt5Widgets - not found")
endif()

# Configuration Options: CMake Standard
option(CMAKE_BUILD_TYPE  "Build as Debug or Release."              Debug)
option(BUILD_SHARED_LIBS "Build Xavi as a shared library."         On)

# Configuration Options: Xavi Specific
option(BUILD_XAVISTA     "Build Xavista, a Qt-based GUI frontend." ${Qt5Widgets_FOUND})
option(BUILD_TESTS       "Build testing programs."                 On)

# Make sure we don't try to build Xavista without Qt5Widgets.
if(${BUILD_XAVISTA} AND NOT ${Qt5Widgets_FOUND})
  message(FATAL_ERROR "Build Xavista requested but Qt5Widgets (required by Xavista) not found.")
endif()

# System-independent values
set(Library_SOURCE_DIR  ${Xavi_SOURCE_DIR}/Xavi)
set(Library_BINARY_DIR  ${Xavi_BINARY_DIR}/Xavi)

# Install locations and system-dependent values.
if(WIN32)
  set(MATH_LIB)
  set(RUNTIME_INSTALL_PATH .)
  set(ARCHIVE_INSTALL_PATH .)
  set(LIBRARY_INSTALL_PATH .)
  set(BUNDLE_INSTALL_PATH Error)
  set(FRAMEWORK_INSTALL_PATH Error)
  set(HEADER_INSTALL_PATH ./include)
  set(DOC_INSTALL_PATH .)
elseif(APPLE)
  set(MATH_LIB)
  set(RUNTIME_INSTALL_PATH bin)
  set(ARCHIVE_INSTALL_PATH Error)
  set(LIBRARY_INSTALL_PATH Error)
  set(BUNDLE_INSTALL_PATH .)
  set(FRAMEWORK_INSTALL_PATH .)
  set(HEADER_INSTALL_PATH Error)
  set(DOC_INSTALL_PATH .)
elseif(UNIX)
  set(MATH_LIB m)
  set(RUNTIME_INSTALL_PATH bin)
  set(ARCHIVE_INSTALL_PATH lib)
  set(LIBRARY_INSTALL_PATH lib)
  set(BUNDLE_INSTALL_PATH share/Xavi/error)
  set(FRAMEWORK_INSTALL_PATH share/Xavi/error)
  set(HEADER_INSTALL_PATH include)
  set(DOC_INSTALL_PATH share/Xavi/docs)
endif()

# CPack Settings

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Xavi_SOURCE_DIR}/Readme.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Xavi is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Xavista Xavista)
set(CPACK_PACKAGE_VERSION_MAJOR ${Xavi_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Xavi_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Xavi_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/")
set(CPACK_RESOURCE_FILE_README "${Xavi_SOURCE_DIR}/Readme.txt")

if(BUILD_XAVISTA)
  set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_PATH})
endif()

if(WIN32)
  set(CPACK_GENERATOR NSIS)
  if(BUILD_XAVISTA)
    set(CPACK_RESOURCE_FILE_LICENSE "${Xavi_SOURCE_DIR}/GPL-3.0.txt")
  else()
    set(CPACK_RESOURCE_FILE_LICENSE "${Xavi_SOURCE_DIR}/LGPL-3.0.txt")
  endif()
elseif(APPLE)
  set(CPACK_GENERATOR DragNDrop)
endif()


# Subdirectory inclusions
add_subdirectory(Xavi)
add_subdirectory(Xavi++)
add_subdirectory(Xavi-Cli)
add_subdirectory(Xavi++-Cli)
if(BUILD_XAVISTA)
  add_subdirectory(Xavi++-Gui-Qt)
endif(BUILD_XAVISTA)
if(BUILD_TESTS)
  add_subdirectory(Tests)
endif(BUILD_TESTS)
install(
  FILES Readme.txt GPL-3.0.txt LGPL-3.0.txt
  DESTINATION ${DOC_INSTALL_PATH})

# This command gets its own section at the end because it's special.
include(CPack)
