CC = gcc
CFLAGS = -Wall -g 

inco:
	$(CC) $(CFLAGS) -o .\bin\inco.exe .\src\inco.c -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

clean:
ifeq ($(OS),Windows_NT)
	del /F .\bin\inco.exe
else
	rm -f .\bin\inco.exe
endif

all: inco

clean_build_all: clean all