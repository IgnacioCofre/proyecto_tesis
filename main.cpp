#include <iostream>
#include <fstream>
#include <string>
#include "includes/class.h"
#include "includes/ants.h"

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
    bool validation_process = false;

    if(show_data_clases)
    {
        data.show_data();
        
        authors.show_data();
        sessions.show_data();
        articles.show_data();
        topics.show_data();

        new_solution.show_solution();
    }

    if(validation_process)
    {
        Validator validator;
        std::cout << "Validating solution" << std::endl;

        validator.articles_in_diferent_sessions(data, authors, new_solution);
        validator.article_assignment(data,new_solution);
        validator.capacity_session(sessions,new_solution);
        validator.capacity_topics(topics,new_solution);
        validator.show_comments();

        validator.quality_solution(articles,new_solution);
    }

    /*Parametros de las hormigas*/
    int number_ants = 1;
    float max_pheromone = 1;
    float min_pheromone = 0.2;
    int number_articles = articles.get_number_articles();
    float alpha = 0.5;
    float beta = 0.5;
    float Lk_constant = 1;

    /*Parametros generales*/
    bool solution_construction = false;
    srand(time(0));
    
    //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant)
    Ants ants = Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant,articles.get_similarity_matrix());
    
    std::vector<int>available_articles(number_articles); 
    std::vector<int>solution;
    for(int iter=0; iter<number_articles; iter++)
    {
        available_articles[iter] = iter;
    }
    
    int initial_article = 0;
    int article_id_1 = available_articles[initial_article];
    solution.push_back(article_id_1);
    available_articles.erase(available_articles.begin()+initial_article);

    while(available_articles.size()>0)
    {
        int next_article = ants.get_next_article(article_id_1,available_articles);
        
        int next_id_article = available_articles[next_article];
        
        
        solution.push_back(next_id_article);
        available_articles.erase(available_articles.begin()+next_article);
        
        if(solution_construction)
        {
            std::cout << articles.get_similarity(article_id_1,next_id_article) <<std::endl;
            for (const int& i : solution) {
                std::cout << i << ",";
            }
            std::cout << std::endl;
            
            for (const int& i : available_articles) {
                std::cout << i << ",";
            }
            std::cout << std::endl;
        }

        article_id_1 = next_id_article;

    }

    std::cout <<"Solution: "<<std::endl;
    for (const int& i : solution) {
        std::cout << i << ",";
    }
    std::cout << std::endl;
    
    return 0;

}