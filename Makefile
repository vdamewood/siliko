PYTHONPATH=/usr/include/python2.7
LIBSUFFIX=.so
NAMEPREFIX=vldc

all: libvldc${LIBSUFFIX} vldc vldc.py _vldc${LIBSUFFIX}

clean:
	rm -f parser.h tokenizer.h
	rm -f parser.c tokenizer.c _vldc.c
	rm -f parser.o tokenizer.o _vldc.o vldc.o main.o

distclean: clean
	rm -f vldc libvldc${LIBSUFFIX} _vldc${LIBSUFFIX} vldc.py vldc.pyc vldc.pyo

buildcheck: all distclean

parser.c: parser.y
	bison --name-prefix=$(NAMEPREFIX) --output=parser.c --defines=parser.h parser.y

parser.h: parser.c

parser.o: parser.c
	gcc -Wall -fPIC -c -o parser.o parser.c

tokenizer.c: tokenizer.l
	flex --prefix=$(NAMEPREFIX) --outfile=tokenizer.c --header-file=tokenizer.h tokenizer.l

tokenizer.h: tokenizer.c

tokenizer.o: tokenizer.c parser.h parserx.h
	gcc -Wall -fPIC -c -o tokenizer.o tokenizer.c

vldc.o: tokenizer.h vldc.c
	gcc -Wall -fPIC -c -o vldc.o vldc.c

main.o: tokenizer.h main.c
	gcc -Wall -c -o main.o main.c

libvldc$(LIBSUFFIX): parser.o tokenizer.o vldc.o
	gcc -Wall -shared -o libvldc$(LIBSUFFIX) vldc.o tokenizer.o parser.o -ly

vldc: libvldc$(LIBSUFFIX) main.o
	gcc -Wall -o vldc main.o -L. -lvldc

_vldc.c: vldc.h
	swig -python -module vldc -o _vldc.c vldc.h

vldc.py: _vldc.c

_vldc.o: _vldc.c vldc.h
	gcc -Wall -fPIC -c -o _vldc.o -I$(PYTHONPATH) _vldc.c

_vldc$(LIBSUFFIX): _vldc.o
	gcc -shared -o _vldc$(LIBSUFFIX) _vldc.o -L. -lvldc

.PHONY: all clean distclean check
