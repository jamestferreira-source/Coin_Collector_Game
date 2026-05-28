# Compiler and flags
CC = gcc
CFLAGS = -Wall
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

# Source and output
SRC = main.c
OUT = coin_collector.exe

# Default target — runs when you type `make`
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

# `make run` — compile and run
run: $(OUT)
	./$(OUT)

# `make clean` — delete compiled files
clean:
	del $(OUT)