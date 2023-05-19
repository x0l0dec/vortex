CC=cc
CFLAGS=-Wall -O3 -lm -pthread
# todo: засунуть это в основную программу
CDIRS=-I. -Iexternal -Icommon
# todo: дважды повторяются строчкой выше и строчкой ниже
EXTERNAL=external/bessel.c external/getch.c external/kbhit.c

.PHONY: all clean

all: vortex

vortex:
	$(CC) -o vortex $(CFLAGS) $(CDIRS) $(EXTERNAL) main.c

clean:
	rm vortex
