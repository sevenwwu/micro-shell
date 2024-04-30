
# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wall

# Define the target executable name
TARGET=ush

# Define the directory for binaries
BINDIR=bin

# Define object files
OBJS=$(BINDIR)/ush.o $(BINDIR)/expand.o

# Default target
all: directories $(BINDIR)/$(TARGET)

# Ensure the bin directory exists
directories:
	mkdir -p $(BINDIR)

# Link the target with object files
$(BINDIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$(TARGET) $(OBJS)

# Compile ush.c with defn.h dependency
$(BINDIR)/ush.o: ush.c defn.h
	$(CC) $(CFLAGS) -c ush.c -o $(BINDIR)/ush.o

# Compile expand.c with defn.h dependency
$(BINDIR)/expand.o: expand.c defn.h
	$(CC) $(CFLAGS) -c expand.c -o $(BINDIR)/expand.o

# Clean target
clean:
	rm -rf $(BINDIR)