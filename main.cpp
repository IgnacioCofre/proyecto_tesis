#include <iostream>
#include <fstream>
#include <string>
#include "includes/class.h"

int main() {
    
    const char * input_file = "input/ebl_one_author/ebl_one_author_30_1.txt";
    const char * solution_test_file = "output/output_example.txt";

    Data data;
    data.read_input_file(input_file);
    
    Solutions new_solution;
    new_solution.read_solution(solution_test_file);
    
    Authors authors = data.create_Authors();
    Sessions sessions = data.create_Sessions();
    Articles articles =data.create_Articles();
    Topics topics = data.create_Topics();
    
    bool show_data_clases = false;
    if(show_data_clases)
    {
        data.show_data();
        
        authors.show_data();
        sessions.show_data();
        articles.show_data();
        topics.show_data();

        new_solution.show_solution();
    }

    bool validation_process = true;
    if(validation_process)
    {
        Validator validator;
        std::cout << "Validating solution" << std::endl;

        validator.articles_in_diferent_sessions(data, authors, new_solution);
        validator.article_assignment(data,new_solution);
        validator.capacity_session(sessions,new_solution);
        validator.capacity_topics(topics,new_solution);
        validator.quality_solution(articles,new_solution);
        validator.show_comments();
    }

    return 0;
    
}