all:
	gcc ./src/pacman.c -o ./bin/game -lcurses

run:
	./bin/game

clean:
	rm ./bin/*
	rm ./map/result.txt