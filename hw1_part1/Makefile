#   whose source is exactly one .c file under linux.
#   check the location of the X libraries on your implementation.
#
# To use this Makefile, you type:
#
#        make xxxx
#                  
# where
#       xxxx.c is the name of the file you wish to compile 
#       
# A binary named xxxx will be produced
# Libraries are assumed to be in the default search paths
# as are any required include files

CC = gcc 

LDLIBS = -lglut -lGL -lGLU -lm

.c:
	$(CC) -w  $@.c $(LDLIBS) -o $@


