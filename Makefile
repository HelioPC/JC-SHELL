CFLAGS = -g -c -std=c18 -Wall -Wpedantic -Wextra

all: jc-shell fibonacci div0

jc-shell: jc-shell.o commandlinereader.o ourfunctions.o list.o
	gcc -o jc-shell commandlinereader.o jc-shell.o ourfunctions.o list.o -pthread

commandlinereader.o: commandlinereader.h
	gcc $(CFLAGS) commandlinereader.c

jc-shell.o: jc-shell.c ourheadfile.h commandlinereader.h list.h
	gcc $(CFLAGS) jc-shell.c

list.o: list.c list.h
	gcc $(CFLAGS) list.c

ourfunctions.o: ourfunctions.c ourheadfile.h list.h
	gcc $(CFLAGS) ourfunctions.c

div0: div0.o
	gcc -o div0 div0.o

div0.o: div0.c
	gcc $(CFLAGS) div0.c

fibonacci: fibonacci.o
	gcc -o fibonacci fibonacci.o

fibonacci.o: fibonacci.c
	gcc $(CFLAGS) fibonacci.c



clean:
	rm -f *.o jc-shell fibonacci div0
run:
	clear
	./jc-shell < input.txt 
