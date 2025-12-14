all:
	mkdir -p ./bin
	gcc ./src/pacman.c -o ./bin/game -lncurses

run:
	./bin/game

clean:
	rm ./bin/*
	rm ./map/result.txt
