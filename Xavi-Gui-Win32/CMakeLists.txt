# Xavi-GUI-Win32

include_directories(${Xavi_SOURCE_DIR})


add_executable(Xavi-Gui-Win32 WIN32 Xavi-Gui-Win32.c Menu.rc About.rc)
target_link_libraries(Xavi-Gui-Win32 User32 Xavi)

install(TARGETS Xavi-Gui-Win32
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH}
)
