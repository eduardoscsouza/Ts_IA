all:
	g++ hanoi.cpp -std=c++11 -O3 -o hanoi.out 

clean:
	rm *.o *.out

run:
	./hanoi.out

test:
	g++ hanoi.cpp -std=c++11 -Wall -Wextra -g -o hanoi.out
	valgrind --leak-check=full --track-origins=yes ./hanoi.out