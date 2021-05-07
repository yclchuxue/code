main:main.o f1.o f2.o
	gcc main.o f1.o f2.o -o main    
main.o:main.c def1.h def2.h
	gcc -c main.c
f1.o:f1.c def1.h
	gcc -c f1.c
f2.o:f2.c def2.h
	gcc -c f2.c