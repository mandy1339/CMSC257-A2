all: Demo

Demo: Demo.o MemHandler.o
	gcc -Wall $^ -o $@

Demo.o: Demo.c MemHandler.h
	gcc -Wall -c $<

MemHandler.o: MemHandler.c MemHandler.h
	gcc -Wall -c $<

clean:
	rm -f Demo Demo.o MemHandler.o
