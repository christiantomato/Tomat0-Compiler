exec = tomat0.out #name of executable
sources = $(wildcard src/*.c) #sources are all the .c files in src
headers = $(wildcard src/*.h) #all the .h files in src

flags = -fsanitize=address #asan for memory debugging

#make executable dependent on all sources and headers and main tomat0 file, skip object file creation and link in one step
$(exec): $(sources) $(headers) main.tmt
	gcc $(sources) $(flags) -o $(exec)
  
#make it accessible globally on system, usage: sudo make install
install:
	cp ./tomat0.out /usr/local/bin/tomat0

#remove all compiled and created files
clean:
	-rm *.out
	-rm -rf *.dSYM
	-rm output/*
