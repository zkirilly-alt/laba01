CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = program
OBJS = main.o inout.o string_poly.o test_poly.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.c string_poly.h test_poly.h
	$(CC) $(CFLAGS) -c main.c -o main.o

inout.o: inout.c inout.h
	$(CC) $(CFLAGS) -c inout.c -o inout.o

string_poly.o: string_poly.c string_poly.h
	$(CC) $(CFLAGS) -c string_poly.c -o string_poly.o

test_poly.o: test_poly.c test_poly.h string_poly.h
	$(CC) $(CFLAGS) -c test_poly.c -o test_poly.o

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild