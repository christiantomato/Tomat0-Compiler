#the binary executable the make file is creating
exec = tomat0.out
#listing our sources (all C files in src folder)
sources = $(wildcard src/*.c)
#object files created by compiler, linked together to create the exectuable
objects = $(sources: .c = .o)

#for debugging, using asan for memory problems
flags = -fsanitize=address

#instructions on how to create the exectuable
$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

#how to create the object filesc
#recreates the object files when change detected
%.o: %.c include/%.h 
	gcc -c $(flags) $< -o $@

#make it accessible globally on system
#(sudo make install)
install:
	make
	cp ./tomat0.out /usr/local/bin/tomat0

#clean command to recompile and remove executables and created files
clean:
	-rm *.out
	-rm *.o 
	-rm src/*.o
	-rm -rf *.dSYM
	-rm output/*
	-rm generated_code.s
