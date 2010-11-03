main: main.c bmp.o
	gcc bmp.o main.c -o bmptest.exe
	rm bmp.o

bmp.o: bmp.c bmp.h
	gcc -c bmp.c -o bmp.o
	
