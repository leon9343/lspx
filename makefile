all:
	gcc ./src/*.c -o lpsx 

dbg:
	gcc -ggdb ./src/*.c -o lpsx

# for debugging only, assumes a test.lp file to compile
run:
	gcc ./src/*.c -o lpsx && ./lpsx test.lp psx.c

# for testing purposes
psx:
	wine ccpsx -llibpad -O3 -Xo$80010000 psx.c -opsx.cpe