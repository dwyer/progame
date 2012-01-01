CC=gcc
RM=rm
CFILES=main.c tmx.c
CFLAGS=-c -ansi -pedantic -Wall
LDFLAGS=-lSDL
OBJDIR=obj
SRCDIR=src
EXECUTABLE=game

OBJS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
SRCS=$(addprefix $(SRCDIR)/, $(CFILES))

all: $(OBJDIR) $(SRCS) $(EXECUTABLE)

debug: CC+=-g -DDEBUG
debug: all

$(EXECUTABLE): $(OBJS) $(BINDIR)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR) $(EXECUTABLE)
