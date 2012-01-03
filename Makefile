CC=gcc
RM=rm
CFILES=main.c world.c player.c tmx.c entity.c input.c config.c
CFLAGS=-I/usr/include/lua5.1/ -c -ansi -pedantic
LDFLAGS=-lSDL -llua5.1
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
