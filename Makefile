# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-g -Wall

# Define the target executable
TARGET=ush

# Define object files
OBJS=ush.o expand.o

# Default target
all: $(TARGET)

# Link the target with object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile ush.c with defn.h dependency
ush.o: ush.c defn.h
	$(CC) $(CFLAGS) -c ush.c

# Compile expand.c with defn.h dependency
expand.o: expand.c defn.h
	$(CC) $(CFLAGS) -c expand.c

# Clean target
clean:
	rm -f $(TARGET) $(OBJS)