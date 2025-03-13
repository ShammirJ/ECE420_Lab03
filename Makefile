# Compiler flags
CFLAGS = -fopenmp -g -Wall -lm

# Default target
all: main datagen serial

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

serial: serialized_main.o Lab3IO.o
	gcc $(CFLAGS) -o serial serialized_main.o Lab3IO.o

clean:
	rm -f *.o main datagen serial

cleanall:
	rm -f  *.o *.txt main datagen data_input data_output data_output_serial serialized_main.o serial

zip:
	zip ECE420_Lab03 *.h *.sh datagen.c Lab3IO.c main.c serialized_main.c RREF.py Makefile ReadMe

cleanzip:
	rm *.zip