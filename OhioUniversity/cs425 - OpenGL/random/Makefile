# This sample Makefile allows you to make an OpenGL application
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

#
# Modified by Prof. Chelberg to also allow you to make from a single
# c++ source file.  So if xxxx.cc is the sourcefile, you type make
# xxxx to produce the binary file.

# last-modified: Mon Sep 29 08:50:08 2008 (clarification of comments).

CC = g++

LDLIBS = -L/usr/X11R6/lib -lX11 -lGL -lGLU  -lglut -lm 
#-lXi -lXmu
INCS = -I/usr/local/include/ -L/usr/lib/

clean:
	rm -f simple1 lines simple triangle

.c:
	$(CC)  $@.c $(INCS) $(LDLIBS) -o $@

.cc:
	$(CC)  $@.cc $(INCS) $(LDLIBS) -o $@
