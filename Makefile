all:
	g++ -Wall main.cpp -o main src/Solution.cpp src/Authors.cpp src/Sessions.cpp src/Articles.cpp src/Topics.cpp src/Data.cpp src/Ants.cpp src/Validator.cpp
	./main
clean:
	rm -rf *o main
#   rm -rf output.txt
#   rm -rf logs.txt