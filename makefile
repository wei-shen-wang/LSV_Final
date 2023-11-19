CC=g++
CFLAGS = -Wall -g -std=c++11

BINDIR = bin
TRAGET = $(BINDIR)/obfuscate

SRCS = $(wildcard src/*.cpp)

all: $(TRAGET)

$(TRAGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TRAGET)