#include <iostream>
#include <fstream>
#include <string>
#include <random>

#include "includes/ants.h"
#include "includes/data.h"
#include "includes/articles.h"
#include "includes/authors.h"
#include "includes/sessions.h"
#include "includes/solutions.h"
#include "includes/validator.h"

int main() {
    
    ///const char * input_file = "input/ebl/ebl_200_5.txt";
    const char * input_file = "input/ebl_modified/ebl_modified_200_5.txt";
    //const char * input_file = "input/ebl_one_author/ebl_one_author_30_1.txt";
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
        
        validator.articles_in_diferent_sessions(data, authors, new_solution.get_scheduling());
        validator.article_assignment(data,new_solution);
        validator.capacity_session(sessions,new_solution.get_scheduling());
        validator.capacity_topics(topics,new_solution.get_scheduling());
        validator.show_comments();

        validator.solution_benefit(articles,new_solution.get_scheduling());    
    }

    /*Parametros generales*/
    int number_articles = articles.get_number_articles();
    
    /*Parametros de las hormigas*/
    int number_anthill = 300;
    int number_ants = 30;
    int e = 5;

    /*Parametro de creacion de soluciones*/
    float alpha = 0.9;
    float beta = 0.9;
    
    /*Parametros de la actualizacion de feromona*/
    float vapor = 0.15;
    float c = 0.01;
    float gamma = 1.0;
    float epsilon = 5.0;

    float max_pheromone = 10.0;
    float min_pheromone = 0.1;

    /*Parametros de muestra de datos por pantalla*/
    bool show_solution_construction = false;
    bool show_solution_quality = false;
    bool show_ant_information = false;
    bool show_best_ant = true;
    bool show_solution_improvement = true;
    bool show_very_best_solution = true;
    
    //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant, similarity_matrix)
    Ants ants = Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,articles.get_similarity_matrix(),vapor,c,gamma,epsilon,e);
    
    /*Algoritmo de hormigas*/
    float very_best_solution_quality = 0;
    std::vector<std::vector<std::vector<std::vector<int>>>> very_best_solution;
    bool random_first_article = true;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, number_articles-1); // define the range

    srand(time(0));

    for(int anthill=0; anthill<number_anthill;anthill++)
    {   
        std::cout<<"Conjunto de hormigas: "<<anthill<<std::endl;
        
        std::vector<std::vector<std::vector<std::vector<int>>>> best_solution;
        float best_solution_quality = -1;

        std::vector<int> available_articles; 
        std::vector<int> solution_articles;    

        for(int id_ant=0; id_ant<number_ants;id_ant++)
        {
            
            if(show_ant_information){std::cout<<"Ant: "<<id_ant<<std::endl;}

            for(int iter=0; iter<number_articles; iter++)
            {
                available_articles.push_back(iter);
            }

            int initial_article = 0;

            if(random_first_article)
            {
                initial_article = distr(gen);  
            }
            
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

            std::vector<std::vector<std::vector<std::vector<int>>>> scheduling = ants.create_solution(id_ant,sessions.get_max_article_session(),solution_articles);
            
            std::vector<int>().swap(solution_articles);
            std::vector<int>().swap(available_articles);

            int solution_benefit = validator.solution_benefit(articles,scheduling);
            /*Numero de restricciones no cumplidas de la solucion*/
            int n_articles_parelel_session = validator.articles_in_diferent_sessions(data,authors,scheduling); 
            int n_max_article_day = validator.capacity_topics(topics,scheduling);
            float solution_quality = ants.calculate_quality_solution(solution_benefit,n_articles_parelel_session,n_max_article_day);

            ants.save_solution(scheduling,solution_quality);

            if(show_solution_quality)
            {
                std::cout<<"Solution benefit:               "<<solution_benefit<<std::endl;
                std::cout<<"Pair articles paralel session:  "<<n_articles_parelel_session<<std::endl;
                std::cout<<"Articles over max topic:        "<<n_max_article_day<<std::endl;
                std::cout<<"Solution quality:               "<<solution_quality<<std::endl;
            }

            if(solution_quality>best_solution_quality)
            {
                best_solution_quality = solution_quality;
                best_solution = scheduling;
            }

            std::vector<std::vector<std::vector<std::vector<int>>>>().swap(scheduling);
        }

        if(show_best_ant){
            std::cout<<"Best Ant"<<std::endl; 
            std::cout<<"Solution benefit:               "<<validator.solution_benefit(articles,best_solution)<<std::endl;
            std::cout<<"Pair articles paralel session:  "<<validator.articles_in_diferent_sessions(data,authors,best_solution)<<std::endl;
            std::cout<<"Articles over max topic:        "<<validator.capacity_topics(topics,best_solution)<<std::endl;
            std::cout<<"Solution quality:               "<<best_solution_quality<<std::endl;
        }

        /*actualizacion de la feromona*/
        ants.pheromone_update_list();
        ants.reset_ants();

        if(best_solution_quality>very_best_solution_quality)
        {
            very_best_solution_quality = best_solution_quality;
            very_best_solution = best_solution;

            if(show_solution_improvement)
            {
                std::cout<<"New best solution: "<<very_best_solution_quality<<std::endl;
            }
        }

        std::vector<std::vector<std::vector<std::vector<int>>>>().swap(best_solution);
    }

    if(show_very_best_solution)
    {
        std::cout<<"\nInformation very best solution"<<std::endl;
        int very_best_solution_benefit = validator.solution_benefit(articles,very_best_solution);
        int n_articles_parelel_session = validator.articles_in_diferent_sessions(data,authors,very_best_solution); 
        int n_max_article_day = validator.capacity_topics(topics,very_best_solution);
        
        std::cout<<"Solution benefit:               "<<very_best_solution_benefit<<std::endl;
        std::cout<<"Pair articles paralel session:  "<<n_articles_parelel_session<<std::endl;
        std::cout<<"Articles over max topic:        "<<n_max_article_day<<std::endl;
        std::cout<<"Solution quality:               "<<very_best_solution_quality<<std::endl;
    }

    return 0;
}