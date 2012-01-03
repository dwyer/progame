CC=gcc
RM=rm
CFILES=main.c world.c player.c tmx.c entity.c
CFLAGS=-c -ansi -pedantic
LDFLAGS=-lSDL
OBJDIR=obj
SRCDIR=src
EXECUTABLE=game

OBJS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
SRCS=$(addprefix $(SRCDIR)/, $(CFILES))

all: $(OBJDIR) $(SRCS) $(EXECUTABLE)
	@if [ ! -d $(HOME)/.config/progame ] ; then mkdir $(HOME)/.config/progame ; fi
	@cp -R ./conf/* $(HOME)/.config/progame
	
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
