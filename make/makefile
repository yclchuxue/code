main:main.o f1.o f2.o
	gcc main.o f1.o f2.o -o main
main.o:def1.h def2.h
f1.o:def1.h
f2.o:def2.h
.PHONY:clean
clean:
	-rm -f *.o