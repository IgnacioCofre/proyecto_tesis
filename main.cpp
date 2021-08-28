#include <iostream>
#include <fstream>
#include <string>

#include "includes/ants.h"
#include "includes/data.h"
#include "includes/articles.h"
#include "includes/authors.h"
#include "includes/sessions.h"
#include "includes/solutions.h"
#include "includes/validator.h"

int main() {
    
    //const char * input_file = "input/ebl/ebl_200_5.txt";
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

    Validator validator;
    
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
        std::cout << "Validating solution" << std::endl;

        validator.articles_in_diferent_sessions(data, authors, new_solution);
        validator.article_assignment(data,new_solution);
        validator.capacity_session(sessions,new_solution.get_scheduling());
        validator.capacity_topics(topics,new_solution.get_scheduling());
        validator.show_comments();

        validator.quality_solution(articles,new_solution.get_scheduling());
    }

    srand(time(0));

    /*Parametros de las hormigas*/
    int number_ants = 30;
    float max_pheromone = 10.0;
    float min_pheromone = 0.1;
    int number_articles = articles.get_number_articles();
    float alpha = 0.5;
    float beta = 0.5;
    float Lk_constant = 1.0;

    /*Parametros de muestra de datos*/
    bool show_solution_construction = false;
    bool show_solution_benefit = true;

    //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant, similarity_matrix)
    Ants ants = Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant,articles.get_similarity_matrix());
    
    /*Algoritmo de hormigas*/

    std::vector<int> available_articles; 
    std::vector<int> solution_articles;

    for(int id_ant=0; id_ant<number_ants;id_ant++)
    {
        std::cout<<"Ant: "<<id_ant<<std::endl;

        for(int iter=0; iter<number_articles; iter++)
        {
            available_articles.push_back(iter);
        }

        int initial_article = 0;
        int id_inital_article = available_articles[initial_article];
        //ants.save_solution(id_ant,id_inital_article);
        solution_articles.push_back(id_inital_article);
        available_articles.erase(available_articles.begin()+initial_article);

        while(available_articles.size()>0)
        {
            int next_article = ants.get_next_article(id_inital_article,available_articles);
            int id_next_article = available_articles[next_article];
            
            //ants.save_solution(id_ant,id_next_article);
            solution_articles.push_back(id_next_article);
            available_articles.erase(available_articles.begin()+next_article);
            
            if(show_solution_construction)
            {
                std::cout << articles.get_similarity(id_inital_article,id_next_article) <<std::endl;
                for (const int& i : solution_articles) {
                    std::cout << i << ",";
                }
                std::cout << std::endl;
                
                for (const int& i : available_articles) {
                    std::cout << i << ",";
                }
                std::cout << std::endl;
            }

            id_inital_article = id_next_article;
        }

        //std::vector<std::vector<std::vector<std::vector<int>>>> sheduling_solution = 
        ants.save_solution(id_ant,sessions.get_max_article_session(),solution_articles);
        solution_articles.clear();

        if(show_solution_benefit)
        {
            std::cout <<"Solution benefit: "<<validator.quality_solution(articles,ants.get_solution_ant(id_ant))<<std::endl;
        }
    }

    return 0;
}