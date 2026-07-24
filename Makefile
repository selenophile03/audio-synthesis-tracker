all:
	gcc -Wall -Wextra -std=c99 main.c -o tracker -lm
clean:
	rm -f tracker *.wav
