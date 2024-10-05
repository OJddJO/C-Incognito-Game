CC = gcc
CFLAGS = -Wall -g 

bruh:
	$(CC) $(CFLAGS) -o .\bin\bruh.exe .\src\bruh.c -I include -L lib -lmingw32 -lSDL2main -lSDL2

clean:
ifeq ($(OS),Windows_NT)
	del /F .\bin\bruh.exe
else
	rm -f .\bin\bruh.exe
endif

all: bruh

clean_build_all: clean all