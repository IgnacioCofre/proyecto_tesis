#include <iostream>
#include <fstream>
#include <string>
#include "includes/class.h"

int main() {
    
    const char * input_file = "input/ebl_one_author/ebl_one_author_30_1.txt";
    const char * solution_test_file = "output/output_example.txt";

    Data data;
    data.read_input_file(input_file);
    //data.show_data();

    Solutions new_solution;
    new_solution.read_solution(solution_test_file);
    //new_solution.show_solution();

    Authors authors = data.create_Authors();
    //authors.show_data();

    Sessions sessions = data.create_Sessions();
    //sessions.show_data();

    Validator validator;
    std::cout << "Validating solution" << std::endl;
    
    validator.articles_in_diferent_sessions(data, authors, new_solution);
    validator.article_assignment(data,new_solution);
    validator.capacity_session(sessions,new_solution);
    validator.show_comments();
    

    return 0;
    
}