VERSION	= 1.0
CC	= gcc
CFLAGS	= -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS	= -lm

GEN	= pixelgen.o functions.o myhead.h

all: pixelgen move clean

pixelgen: $(GEN)
	$(CC) $(CFLAGS) $(GEN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

move:
	mv a.out pixelgen

clean:
	find . -name "*.o" -exec rm {} \;
	find . -name "*.h.gch" -exec rm {} \;
