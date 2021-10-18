
CC=gcc
CFLAGS=-g -Wall -Iinclude
LFLAGS=-lcurses -lpanel

SRCDIR=src
OBJDIR=obj
BINDIR=bin
INCDIR=include

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

BIN=ntd

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/todo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJDIR)/* $(BINDIR)/*

run:
	@./$(BINDIR)/$(BIN)
