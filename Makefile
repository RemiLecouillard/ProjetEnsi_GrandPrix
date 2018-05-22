CFLAGS = -Wall -Wextra -pedantic-errors
CC = gcc
LDFLAGS = -lm
SRCDIR = src/
OBJDIR = obj/
LIB = -l/lib/CCollections
SRC = $(wildcard $(SRCDIR)*.c)
OBJ  = $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
outputdir = ../GrandPrix2018/drivers/

.PHONY: clean all

all: lib/libCCollections.a $(outputdir)LecouillardDromard

$(outputdir)LecouillardDromard: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) -Llib/ -lCCollections

lib/libCCollections.a :
	@make -C CCollections/

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(CC) -o $@ -c $< -I include/ -I CCollections/include $(CFLAGS) -O2

clean :
	rm -f $(OBJDIR)*.o $(OBJDIR)*~ lib/*.a

doc :
	doxygen Doxyfile
