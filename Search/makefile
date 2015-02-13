COMPILER = gcc
all: search

debug:
	make DEBUG=TRUE

search: search.o
	$(COMPILER) -o search search.o 
search.o: search.c search.h 
	$(COMPILER) -c search.c
clean:
	rm -rf search
	rm -rf *.o

rebuild: clean all
