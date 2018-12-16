all: setup program1

setup: 
	mkdir mylib

program1: main.c libfactorial.so libfibonacci.so
	gcc -o program1 main.c -lfactorial -lfibonacci -L./mylib

libfactorial.so: factorial.o
	gcc -shared -o mylib/libfactorial.so factorial.o

factorial.o: factorial.c
	gcc -c -fpic -Wall -Werror factorial.c

libfibonacci.so: fibonacci.o
	gcc -shared -o mylib/libfibonacci.so fibonacci.o

fibonacci.o: fibonacci.c
	gcc -c -fpic -Wall -Werror fibonacci.c


run: setup program1
	@echo LD_LIBRARY_PATH $(LD_LIBRARY_PATH)
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):./mylib
	@echo printing LD_LIBRARY_PATH $(LD_LIBRARY_PATH)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):./mylib ./program1
	
run_broken: program1
	@echo LD_LIBRARY_PATH $(LD_LIBRARY_PATH)
	./program1


.PHONY: clean

clean: 
	rm -rf *.o mylib program1 


