all:
	gcc ./src/*.c -o lpsx

dbg:
	gcc -g ./src/*.c -o lpsx

# for debugging only, assumes a test.lp file to compile
run:
	gcc ./src/*.c -o lpsx && ./lpsx test.lp
