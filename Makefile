CC=gcc
RM=rm
CFILES=config.c entities.c entity.c event.c game.c main.c script.c tilemap.c
CFLAGS=-c -ansi -pedantic -Wall -I./include $(shell pkg-config sdl lua --cflags --silence-errors) $(shell pkg-config sdl lua5.1 --cflags --silence-errors)
LDFLAGS=$(shell pkg-config sdl lua --libs --silence-errors) $(shell pkg-config sdl lua5.1 --libs --silence-errors)
OBJDIR=obj
SRCDIR=src
DOCDIR=doc
EXECUTABLE=game

OBJS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
SRCS=$(addprefix $(SRCDIR)/, $(CFILES))

all: $(OBJDIR) $(SRCS) $(EXECUTABLE)

$(DOCDIR): Doxyfile $(SRCS)
	doxygen

debug: CC+=-g -DDEBUG
debug: all

profile: CC+=-pg
profile: all

$(EXECUTABLE): $(OBJS) $(BINDIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(RM) -rf $(EXECUTABLE) $(OBJDIR) $(DOCDIR)
