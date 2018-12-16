all: program1

program1: main.c 
	gcc -std=gnu99 -fPIC -pthread -o program1 main.c

run:
	./program1 input_data.txt 12 20 1

run_valgrind: program1
	valgrind -v --tool=memcheck --leak-check=yes --num-callers=20 --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./program1

.PHONY: clean

clean: 
	rm -f program1
