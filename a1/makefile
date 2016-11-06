cs350sh: LinkedList.o cs350sh.o sig_handler.h
	gcc -o cs350sh cs350sh.o LinkedList.o

LinkedList: LinkedList.o
	gcc -o LinkedList LinkedList.o

LinkedList.o: LinkedList.c LinkedList.h
	gcc -c LinkedList.c 

cs350sh.o: cs350sh.c
	gcc -c cs350sh.c

clean:
	rm cs350sh *.o
