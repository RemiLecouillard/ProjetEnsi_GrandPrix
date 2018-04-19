CFLAGS = -Wall -Wextra -pedantic-errors
CC = gcc
LDFLAGS =
SRCDIR = src/
OBJDIR = obj/
SRC = $(wildcard $(SRCDIR)*.c)
OBJ  = $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
outputdir = ../GrandPrix2018_3.0.3/drivers/

.PHONY: clean all

all: $(outputdir)LecouillardDromard Makefile

$(outputdir)LecouillardDromard: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -o $@ -c $< -I include/ $(CFLAGS)

clean :
	rm -f *.o *~

doc :
	doxygen Doxyfile
