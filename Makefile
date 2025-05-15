CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ = bptree/bptree.o write_file.o main.o 
TARGET = bptree_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

bptree/bptree.o: bptree/bptree.c bptree/bptree.h
	$(CC) $(CFLAGS) -c bptree/bptree.c -o bptree/bptree.o

write_file.o: write_file.c write_file.h
	$(CC) $(CFLAGS) -c write_file.c -o write_file.o

main.o: main.c bptree/bptree.h write_file.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(OBJ) $(TARGET)
