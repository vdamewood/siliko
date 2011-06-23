PYTHONPATH=/usr/include/python2.7
LIBSUFFIX=.so
NAMEPREFIX=vldc_yy
CC=gcc
CFLAGS=-Wall -fPIC
LDFLAGS=-Wall
all: libvldc${LIBSUFFIX} vldc

clean:
	rm -f parser.h tokenizer.h
	rm -f parser.c tokenizer.c 
	rm -f parser.o tokenizer.o parserx.o vldc.o main.o

distclean: clean
	rm -f vldc libvldc${LIBSUFFIX} 

buildcheck: all distclean

parserx.o: parserx.h parserx.c
	$(CC) $(CFLAGS) -c -o parserx.o parserx.c

parser.c: parser.y
	bison --name-prefix=$(NAMEPREFIX) --output=parser.c --defines=parser.h parser.y

parser.h: parser.c

parser.o: parserx.h parser.c tokenizer.h
	$(CC) $(CFLAGS) -c -o parser.o parser.c

tokenizer.c: tokenizer.l
	flex --prefix=$(NAMEPREFIX) --outfile=tokenizer.c --header-file=tokenizer.h tokenizer.l

tokenizer.h: tokenizer.c

tokenizer.o: tokenizer.c parser.h parserx.h
	gcc -Wall -fPIC -c -o tokenizer.o tokenizer.c

vldc.o: tokenizer.h vldc.c
	gcc -Wall -fPIC -c -o vldc.o vldc.c

main.o: tokenizer.h main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

libvldc$(LIBSUFFIX): parserx.o parser.o tokenizer.o vldc.o
	gcc $(LDFLAGS) -shared -o libvldc$(LIBSUFFIX) vldc.o parserx.o tokenizer.o parser.o

vldc: libvldc$(LIBSUFFIX) main.o
	gcc $(LDFLAGS) -o vldc main.o -L. -lvldc
	
.PHONY: all clean distclean check
