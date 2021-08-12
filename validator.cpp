#include <iostream>
#include <fstream>
#include <string>
#include "includes/class.h"

int main() {
    
    //int x = add(2, 4);
    //std::cout << x << std::endl;

    const char * input_file = "input/ebl_one_author/ebl_one_author_30_1.txt";
    const char * solution_test_file = "output/output_example.txt";

    Data data;
    data.read_input_file(input_file);
    //data.show_data();

    Solutions new_solution;
    new_solution.read_solution(solution_test_file);

    return 0;
    
}