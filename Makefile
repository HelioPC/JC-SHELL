CFLAGS = -g -c -std=c18 -Wall -Wpedantic -Wextra

all: jc-shell jcshell-terminal fibonacci div0
	rm -f *.o div0

jc-shell: jc-shell.o commandlinereader.o ourfunctions.o list.o files.o monitorthread.o listterm.o
	gcc -o jc-shell commandlinereader.o jc-shell.o ourfunctions.o list.o files.o monitorthread.o listterm.o -pthread

jcshell-terminal: jcshell-terminal.o ourfunctions.o
	gcc -o jcshell-terminal jcshell-terminal.o ourfunctions.o

commandlinereader.o: headers/commandlinereader.h
	gcc $(CFLAGS) commandlinereader.c

jc-shell.o: jc-shell.c headers/ourheadfile.h headers/commandlinereader.h headers/list.h headers/listterm.h
	gcc $(CFLAGS) jc-shell.c

jcshell-terminal.o: jcshell-terminal.c headers/ourheadfile.h
	gcc $(CFLAGS) jcshell-terminal.c

list.o: list.c headers/list.h
	gcc $(CFLAGS) list.c

ourfunctions.o: ourfunctions.c headers/ourheadfile.h headers/list.h
	gcc $(CFLAGS) ourfunctions.c

monitorthread.o: monitorthread.c
	gcc $(CFLAGS) monitorthread.c

files.o: files.c
	gcc $(CFLAGS) files.c

listterm.o: listterm.c headers/listterm.h
	gcc $(CFLAGS) -D_POSIX_C_SOURCE listterm.c

div0: div0.o
	gcc -o div0 div0.o

div0.o: div0.c
	gcc $(CFLAGS) div0.c

fibonacci: fibonacci.o
	gcc -o fibonacci fibonacci.o

fibonacci.o: fibonacci.c
	gcc $(CFLAGS) fibonacci.c



clean:
	rm -f *.o jc-shell fibonacci div0 jcshell-out-* jcshell-terminal *-in
run:
	clear
	./jc-shell < input.txt 
