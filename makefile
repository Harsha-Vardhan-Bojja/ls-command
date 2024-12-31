# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iinc

# List of object files
OBJS = obj/functions.o obj/main.o

# Name of the final executable
TARGET = bin/output


# Rule to build the final executable
$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile functions.c
obj/functions.o: src/functions.c inc/header.h inc/colors.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/functions.c -o obj/functions.o

# Rule to compile main.c
obj/main.o: src/main.c inc/header.h inc/colors.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o



# Rule to clean up generated files
clean:
	rm -rf obj bin

# Phony targets
.PHONY: clean run

