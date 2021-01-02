FLAGS = -O3 -Wall -std=c11
LIBS = -lm

all: main.o executioner.o simulator.o disk_helper.o irq_helper.o monitor_helper.o tracer.o
	gcc -g main.o executioner.o simulator.o disk_helper.o irq_helper.o monitor_helper.o tracer.o -o simulator $(LIBS)

clean:
	rm -rf *.o simulator

simulator.o: defines.h simulator.c simulator.h
	gcc -g 	$(FLAGS) -c simulator.c

executioner.o: defines.h executioner.c executioner.h
	gcc -g 	$(FLAGS) -c executioner.c

disk_helper.o: defines.h disk_helper.c disk_helper.h
	gcc -g 	$(FLAGS) -c disk_helper.c

irq_helper.o: defines.h irq_helper.c irq_helper.h
	gcc -g 	$(FLAGS) -c irq_helper.c

monitor_helper.o: defines.h monitor_helper.c monitor_helper.h
	gcc -g 	$(FLAGS) -c monitor_helper.c

tracer.o: defines.h tracer.c tracer.h
	gcc -g 	$(FLAGS) -c tracer.c

main.o: main.c simulator.h
	gcc -g $(FLAGS) -c main.c