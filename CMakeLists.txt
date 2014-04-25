# Root

# Semi-Required Preamble
project(Xavi)
cmake_minimum_required(VERSION 2.8)
set(Xavi_VERSION_MAJOR 0)
set(Xavi_VERSION_MINOR 0)
set(Xavi_VERSION_PATCH 0-git)

if(EXISTS ${Xavi_SOURCE_DIR}/GitCommit.txt)
  set(GIT_BUILD On)
  set(GIT_FILE GitCommit.txt)
  file(READ "${Xavi_SOURCE_DIR}/GitCommit.txt" GIT_COMMIT LIMIT 40)
  string(STRIP ${GIT_COMMIT} GIT_COMMIT)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(MACOSX 1)
endif()

# Dependency searches
find_package(BISON 2.6 REQUIRED)
find_package(FLEX 2.5.35)
find_package(Qt4)

# Configuration Options
option(BUILD_XAVISTA
  "Build Xavista, a Qt-based GUI frontend to Xavi"
  ${QT4_FOUND})
option(BUILD_SHARED_LIBS "Build Xavi as a shared library." ON)
option(BUILD_TESTS "Build testing programs." OFF)
option(USE_BISON "Build a parser using GNU Bison" ON)
option(USE_FLEX  "Build a lexer using FLEX" OFF)

# Install locations and system-dependent values.
if(WIN32)
  set(MATH_LIB)
  set(RUNTIME_INSTALL_PATH .)
  set(ARCHIVE_INSTALL_PATH .)
  set(LIBRARY_INSTALL_PATH .)
  set(BUNDLE_INSTALL_PATH Error)
  set(FRAMEWORK_INSTALL_PATH Error)
  set(HEADER_INSTALL_PATH .)
  set(DOC_INSTALL_PATH .)
elseif(MACOSX)
  set(MATH_LIB)
  set(RUNTIME_INSTALL_PATH Error)
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
  set(BUNDLE_INSTALL_PATH share/xavi/error)
  set(FRAMEWORK_INSTALL_PATH share/xavi/error)
  set(HEADER_INSTALL_PATH include)
  set(DOC_INSTALL_PATH share/xavi/docs)
endif()

# CPack Settings

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Xavi_SOURCE_DIR}/Readme.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Xavi is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Xavista Xavista)
set(CPACK_PACKAGE_VERSION_MAJOR ${Xavi_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Xavi_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Xavi_VERSION_PATCH})
if(GIT_BUILD)
  message("Setting PACKAGE version")
  set(CPACK_PACKAGE_VERSION "git.${GIT_COMMIT}")
endif()
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/build/")
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
elseif(MACOSX)
  set(CPACK_GENERATOR DragNDrop)
endif()


# Subdirectory inclusions
add_subdirectory(Xavi)
if(BUILD_XAVISTA)
  add_subdirectory(Xavista)
endif(BUILD_XAVISTA)
if(BUILD_TESTS)
  add_subdirectory(Tests)
endif(BUILD_TESTS)
install(
  FILES Readme.txt GPL-3.0.txt LGPL-3.0.txt ${GIT_FILE}
  DESTINATION ${DOC_INSTALL_PATH})

# This command gets its own section at the end because it's special.
include(CPack)
