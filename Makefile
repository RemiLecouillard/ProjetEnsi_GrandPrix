CFLAGS = -Wall -Wextra -pedantic-errors
CC = gcc
LDFLAGS =
SRCDIR = src/
OBJDIR = obj/
LIB = -l/lib/CCollections
SRC = $(wildcard $(SRCDIR)*.c)
OBJ  = $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
outputdir = ../GrandPrix2018/drivers/

.PHONY: clean all

all: $(outputdir)LecouillardDromard Makefile

$(outputdir)LecouillardDromard: $(OBJ) lib/CCollections.a
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -o $@ -c $< -I include/ -I CCollections/include $(CFLAGS)

clean :
	rm -f $(OBJDIR)*.o $(OBJDIR)*~

doc :
	doxygen Doxyfile
