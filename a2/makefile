cs350sh: cs350sh.o parser.o cmd_node.o str_node.o proc_node.o utils.o
	gcc -g -o cs350sh cs350sh.o parser.o cmd_node.o str_node.o utils.o proc_node.o

cs350sh.o: cs350sh.h cs350sh.c 
	gcc -g -c cs350sh.c

proc_node: proc_node.o
	gcc -g -o proc_node proc_node.o

proc_node.o : proc_node.h proc_node.c
	gcc -g -c proc_node.c

str_node: str_node.o
	gcc -g -o str_node str_node.o

str_node.o : str_node.h str_node.c
	gcc -g -c str_node.c

cmd_node: cmd_node.o
	gcc -g -o cmd_node cmd_node.o

cmd_node.o: cmd_node.c cmd_node.h
	gcc -g -c cmd_node.c

parser: cmd_node.o str_node.o parser.o 
	gcc -g -o parser parser.o str_node.o cmd_node.o

parser.o: parser.c parser.h
	gcc -g -c parser.c

utils.o: utils.c utils.h
	gcc -g -c utils.c


	
clean:
	rm -f *.o *gch cs350sh
