CC=gcc

build: tema1

tema1: tema1.c functii.h
	$(CC) tema1.c -o tema1

.PHONY: clean

clean:
	rm -f tema1
