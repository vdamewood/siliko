lib_LIBRARIES=libvldc.a
libvldc_a_SOURCES=tokenizer.l parser.y parserx.c libvldc.c parserx.h libvldc.h

NAMEPREFIX=vldc_yy
AM_CFLAGS=-Wall -fPIC
AM_YFLAGS=--name-prefix=$(NAMEPREFIX) --defines=parser.h
AM_LFLAGS=--prefix=$(NAMEPREFIX) --header-file=tokenizer.h

parser.h: parser.c

tokenizer.h: tokenizer.c

parser.c: parser.y
	bison $(AM_YFLAGS) --output=parser.c parser.y

tokenizer.c: tokenizer.l
	flex $(AM_LFLAGS) --outfile=tokenizer.c tokenizer.l

parser.o: parserx.h parser.c tokenizer.h
	$(CC) $(AM_CFLAGS) $(CFLAGS) -c -o parser.o parser.c

tokenizer.o: tokenizer.c parser.h parserx.h
	$(CC) $(AM_CFLAGS) $(CFLAGS) -c -o tokenizer.o tokenizer.c

libvldc$(LIBSUFFIX): parserx.o parser.o tokenizer.o vldc.o
	$(LD) -shared -o libvldc$(LIBSUFFIX) libvldc.o parserx.o tokenizer.o parser.o
