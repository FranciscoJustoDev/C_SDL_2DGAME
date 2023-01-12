CC = gcc
CFLAGS = -lSDL2 -I$(IDIR)

IDIR = ./
SRCDIR = ./

SOURCES = $(SRCDIR)*.c

NAME = Game

all: compile run clean

compile:
	$(CC) $(SOURCES) $(CFLAGS) -o $(NAME)

run:
	./$(NAME)

clean:
	rm $(NAME)
