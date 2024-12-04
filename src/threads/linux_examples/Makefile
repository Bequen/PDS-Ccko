examples: examples.o
	gcc -o examples examples.o -pthread

examples.o: examples.c
	gcc -c examples.c -Wall -pthread

clean:
	rm -f *.o examples
