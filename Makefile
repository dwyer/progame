CC=gcc
RM=rm
CFILES=main.c world.c player.c tmx.c entity.c script.c event.c config.c
CFLAGS=-c -ansi -pedantic $(shell sdl-config --cflags) $(shell pkg-config lua --cflags --silence-errors) $(shell pkg-config lua5.1 --cflags --silence-errors)
LDFLAGS=$(shell sdl-config --libs) $(shell pkg-config lua --libs --silence-errors) $(shell pkg-config lua5.1 --libs --silence-errors)
OBJDIR=obj
SRCDIR=src
EXECUTABLE=game

OBJS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
SRCS=$(addprefix $(SRCDIR)/, $(CFILES))

all: $(OBJDIR) $(SRCS) $(EXECUTABLE)

debug: CC+=-g -DDEBUG
debug: all

$(EXECUTABLE): $(OBJS) $(BINDIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(RM) -rf $(OBJDIR) $(EXECUTABLE)
