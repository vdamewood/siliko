# Xavita

include_directories(${Library_SOURCE_DIR})

if (UNIX)
  add_definitions(-DXAVITA_USE_UNIX=1)
else()
  add_definitions(-DXAVITA_USE_UNIX=0)
endif()

add_executable(Xavita Xavita.c)
target_link_libraries(Xavita Xavi)

install(TARGETS Xavita
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH}
)
