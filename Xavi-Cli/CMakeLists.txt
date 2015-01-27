# Xavita

include_directories(${Xavi_SOURCE_DIR})

if (UNIX)
  add_definitions(-DXAVITA_USE_UNIX=1)
else()
  add_definitions(-DXAVITA_USE_UNIX=0)
endif()

add_executable(Xavi-Cli Xavi-Cli.c)
target_link_libraries(Xavi-Cli Xavi)

install(TARGETS Xavi-Cli
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH}
)
