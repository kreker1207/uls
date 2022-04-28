CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

OBJ_DIR = obj
SRC_DIR = src
INC_DIR = inc
LIB_DIR = libmx
EXENAME = uls

SRC = $(SRC_DIR)/*.c

ROOT_OBJS = *.o

OBJS = $(OBJ_DIR)/*.o 

.PHONY: all clean uninstall reinstall

all:
	make -sC $(LIB_DIR)
	make EXENAME

EXENAME:
	mkdir obj
	$(CC) $(CFLAGS) -c $(SRC)
	mv $(ROOT_OBJS) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_DIR)/libmx.a -o $(EXENAME)

clean:
	rm -rf obj
	rm -rf $(LIB_DIR)/obj

uninstall:
	make clean
	rm -rf $(LIB_DIR)/libmx.a
	rm -rf $(EXENAME)

reinstall:
	make uninstall
	make all

