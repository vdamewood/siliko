cmake_minimum_required (VERSION 2.6)
OPTION(BUILD_SHARED_LIBS "Build shared library." ON)
project (vldc)
add_custom_command (
  OUTPUT tokenizer.h tokenizer.c
  COMMAND flex --nounistd --prefix=vldc_yy --header-file=tokenizer.h --outfile=tokenizer.c tokenizer.l
  DEPENDS tokenizer.l
  )
add_custom_command (
  OUTPUT parser.h parser.c
  COMMAND bison --name-prefix=vldc_yy --defines=parser.h --output=parser.c parser.y
  DEPENDS parser.y
)

add_library(vldc libvldc.c parserx.c parser.c tokenizer.c)
