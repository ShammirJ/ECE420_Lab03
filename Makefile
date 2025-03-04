# Compiler flags
CFLAGS = -fopenmp -Wall -O2 -lm

# Default target
all: main datagen

# Rule to compile the executable
main: main.o Lab3IO.o
	gcc $(CFLAGS) -o main main.o Lab3IO.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

Lab3IO.o: Lab3IO.c
	gcc $(CFLAGS) -c Lab3IO.c

datagen: datagen.o Lab3IO.o
	gcc $(CFLAGS) -o datagen datagen.o Lab3IO.o

datagen.o: datagen.c
	gcc $(CFLAGS) -c datagen.c

clean:
	rm -f main.o Lab3IO.o datagen.o main datagen

clean all:
	rm -f main.o Lab3IO.o datagen.o main datagen data_input data_output

