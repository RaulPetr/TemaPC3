build: bmp_header.h
	gcc -o bmp tema3.c
run: bmp
	./bmp
clean: bmp
	rm bmp