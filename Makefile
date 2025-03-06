# Compiler flags
CFLAGS = -fopenmp -g -Wall -O2 -lm

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

serialized_main.o: serialized_main.c
	gcc $(CFLAGS) -c serialized_main.c

Lab3IO_copy.o: Lab3IO_copy.c
	gcc $(CFLAGS) -c Lab3IO_copy.c

serial: serialized_main.o Lab3IO_copy.o
	gcc $(CFLAGS) -o serial serialized_main.o Lab3IO_copy.o

clean:
	rm -f main.o Lab3IO.o datagen.o main datagen

cleanall:
	rm -f main.o Lab3IO.o datagen.o main datagen data_input data_output Lab3IO_copy.o data_output_serial serialized_main.o serial

