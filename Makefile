all:
	g++ -Wall main.cpp -o main includes/function.cpp includes/class.h
	./main
clean:
	rm -rf *o main
#   rm -rf output.txt
#   rm -rf logs.txt