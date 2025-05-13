CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ = bptree/bptree.o main.o
TARGET = bptree_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

bptree/bptree.o: bptree/bptree.c bptree/bptree.h
	$(CC) $(CFLAGS) -c bptree/bptree.c -o bptree/bptree.o

main.o: main.c bptree/bptree.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(OBJ) $(TARGET)
