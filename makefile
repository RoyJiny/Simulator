FLAGS = -O3 -Wall -std=c11
LIBS = -lm

all: main.o executioner.o simulator.o
	gcc -g main.o executioner.o simulator.o -o simulator $(LIBS)

clean:
	rm -rf *.o simulator

simulator.o: defines.h simulator.c simulator.h
	gcc -g 	$(FLAGS) -c simulator.c

executioner.o: defines.h executioner.c executioner.h
	gcc -g 	$(FLAGS) -c executioner.c

main.o: main.c simulator.h
	gcc -g $(FLAGS) -c main.c