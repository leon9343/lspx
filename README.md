# lspx

Very basic scripting language that produce a working psx executable. This was made for learning purposes and isn't intended for actual game development.
Currently it is able to render basic shapes, but support for more advanced features will be added later.
Below there is a basic guide on how to get something on screen

# drawing a triangle

We first need to create a target .c file and a .lp script file. Then we edit the .lp file like this:
```
# THIS IS A COMMENT #

# These are special functions that don't require a terminating ; #
init graphics
def POLY_F3 triangle

# These will define the parameters of the triangle #
setPolyF3 triangle ;
setRGB0 triangle 255 255 255 ;
setXY3 triangle 100 100 75 100 125 100 ;
```
Save the file and run
```
./lspx script.ls file.c
```
Now the program will compile the generated file.c into a .cpe file with a white triangle drawn. 

# dependencies
In order to run this program you need the psyq sdk (available on http://www.psxdev.net/downloads.html) and a makefile with these contents:
```
psx:
	wine ccpsx -O3 -Xo$80010000 file.c -opsx.cpe
```
Which will output a psx.cpe file. Instead of file.c put the name of your target c file
