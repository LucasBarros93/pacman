all:
	mkdir -p ./bin
	gcc -O2 -static -s ./src/pacman.c -o ./bin/game -lncursesw -ltinfo

run:
	./bin/game

clean:
	rm ./bin/*
	rm ./map/result.txt
