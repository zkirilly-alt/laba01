CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
AR = ar
ARFLAGS = rcs

LIB_DIR = lib
LIBRARY = $(LIB_DIR)/libstring.a

TARGET = program
TEST_TARGET = test_program

OBJS = string_poly.o
IO_OBJS = inout.o
TEST_OBJS = tests/test_poly.o tests/test_main.o

all: $(LIB_DIR) $(LIBRARY) $(TARGET)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(TARGET): main.o $(IO_OBJS) $(LIBRARY)
	$(CC) main.o $(IO_OBJS) -L$(LIB_DIR) -lstring -o $@

$(TEST_TARGET): $(TEST_OBJS) $(LIBRARY)
	$(CC) $(TEST_OBJS) -L$(LIB_DIR) -lstring -o $@

main.o: main.c inout.h string_poly.h
	$(CC) $(CFLAGS) -c main.c -o main.o

string_poly.o: string_poly.c string_poly.h
	$(CC) $(CFLAGS) -c string_poly.c -o string_poly.o

inout.o: inout.c inout.h string_poly.h
	$(CC) $(CFLAGS) -c inout.c -o inout.o

tests/test_poly.o: tests/test_poly.c test_poly.h string_poly.h
	$(CC) $(CFLAGS) -c tests/test_poly.c -o tests/test_poly.o

tests/test_main.o: tests/test_main.c test_poly.h
	$(CC) $(CFLAGS) -c tests/test_main.c -o tests/test_main.o

test: $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(IO_OBJS) main.o $(LIBRARY) $(TARGET)
	rm -f $(TEST_OBJS) $(TEST_TARGET)
	rmdir $(LIB_DIR) 2>/dev/null || true


.PHONY: all clean run rebuild test rebuild_test