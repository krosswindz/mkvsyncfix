#
# By Bharath Ramesh,	2007	Makefile
# © Copyrights 2007, Bharath Ramesh. All rights reserved.
#

# set compiler to be used
CC = i586-mingw32msvc-gcc
STRIP = i586-mingw32msvc-strip
WINDRES = i586-mingw32msvc-windres

# set compile time flags
C99 = -std=c99
CFLAGS = -Wall -Wextra -O3
GNU = -D_GNU_SOURCE
LIBRARY = -lpdcurses

# set object files needed
OBJS = mkvsyncfix.o

# set the target
TARGET = mkvsyncfix.exe

# compilation step
%.o : %.c
	$(CC) $(C99) $(CFLAGS) $(GNU) -c $<

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

mkvsyncfix.exe: mkvsyncfix.o
	$(CC) -o $@ $? $(LIBRARY)
	$(STRIP) -s $@
