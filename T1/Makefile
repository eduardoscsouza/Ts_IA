all:
	g++ hanoi.cpp -std=c++11 -O3 -o hanoi.out 

clean:
	rm *.o *.out

run:
	./hanoi.out 10 3

test:
	g++ hanoi.cpp -std=c++11 -Wall -Wextra -g -o hanoi.out
	valgrind --leak-check=full --track-origins=yes ./hanoi.out 10 3