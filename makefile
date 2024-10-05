CC = gcc
CFLAGS = -Wall -g

bruh:
	$(CC) $(CFLAGS) -o .\src\bruh.exe .\src\bruh.c

clean:
ifeq ($(OS),Windows_NT)
	del /F .\src\bruh.exe
else
	rm -f .\src\bruh.exe
endif

all: bruh

clean_build_all: clean all