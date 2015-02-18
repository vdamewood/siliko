# Xavita

include_directories(${Xavi_SOURCE_DIR})

if (UNIX)
  add_definitions(-DXAVITA_USE_UNIX=1)
else()
  add_definitions(-DXAVITA_USE_UNIX=0)
endif()

set(SOURCES XaviPP-Cli.cpp StringSource.cpp)
set(HEADERS StringSource.cpp)

add_executable(XaviPP-Cli ${HEADERS} ${SOURCES})
target_link_libraries(XaviPP-Cli XaviPP)

install(TARGETS XaviPP-Cli
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH}
)
