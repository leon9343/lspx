all:
	gcc ./src/*.c -o lpsx

dbg:
	gcc -g ./src/*.c -o lpsx
