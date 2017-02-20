
_DEPS=$(shell ls *.c) $(shell ls dep/*.c)
DEPS_SRC=$(subst extractor.c ,,$(_DEPS))
DEPS=$(patsubst %.c,%.o,$(DEPS_SRC))

CFLAGS=-I/usr/include/SDL2 -I/usr/include/i386-linux-gnu
LIBS=-lSDL2 -lm -lSDL2_ttf -lSDL2_image

all: opengadget

opengadget: $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o ; rm -f dep/*.o

