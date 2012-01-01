CC=gcc
RM=rm
CFILES=main.c tmx.c
CFLAGS=-c -ansi -pedantic -Wall
LDFLAGS=-lSDL
BINDIR=bin/
OBJDIR=obj/
SRCDIR=src/
EXECUTABLE=bin/game

OBJECTS=$(addprefix $(OBJDIR), $(CFILES:.c=.o))
SOURCES=$(addprefix $(SRCDIR), $(CFILES))

all: $(SOURCES) $(EXECUTABLE)

debug: CC+=-g -DDEBUG
debug: all

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
