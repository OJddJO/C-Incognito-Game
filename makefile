CC = gcc
CFLAGS = -Wall -g
TARGET = ./src/bruh.exe
SRC = ./src/bruh.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)