XX = g++

all: main

# Compil files

main: main.cpp ./Global/*.cpp ./Fasta/*.cpp
	clear
	$(CXX) -Wall -Wextra -Werror -ansi -pedantic -std=c++14 -o main $^



# Start the program
run: main
	./main

startDoxygen:
	doxywizard &

doxy:
	doxygen Doxyfile  
	firefox ./doc/html/index.html	