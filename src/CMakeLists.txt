cmake_minimum_required (VERSION 2.6)
OPTION(BUILD_SHARED_LIBS "Build shared library." ON)
project (vldc)

add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.h ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.c
  COMMAND flex --nounistd --prefix=vldc_yy --header-file=${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.h --outfile=${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.c ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.l
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.l
  )
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/parser.h ${CMAKE_CURRENT_SOURCE_DIR}/parser.c
  COMMAND bison --name-prefix=vldc_yy --defines=${CMAKE_CURRENT_SOURCE_DIR}/parser.h --output=${CMAKE_CURRENT_SOURCE_DIR}/parser.c ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
)

add_library(vldc vldc.c parser.c tokenizer.c)
