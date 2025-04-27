CC=gcc
CFLAGS=-Iinclude -Wall -Wextra -g
LDFLAGS=-lpthread
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
TARGET=hamburgueseria

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o
