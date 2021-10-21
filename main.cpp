#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>

#include "includes/ants.h"
#include "includes/data.h"
#include "includes/articles.h"
#include "includes/authors.h"
#include "includes/sessions.h"
#include "includes/solutions.h"
#include "includes/validator.h"
#include "includes/improvement.h"

using namespace std::chrono;

int main() {
    
    /*Archivo input*/
    //std::string input_name = "lars/lars_original.txt";
    //std::string input_name = "ebl/ebl_original.txt";
    
    //std::string input_name = "lars/lars_30_5.txt";
    //std::string input_name = "ebl/ebl_30_5.txt";
    
    //std::string input_name = "ebl_modified/ebl_modified_200_5.txt";

    //std::string input_name = "ebl/ebl_200_5.txt";
    //std::string input_name = "lars/lars_100_2.txt";

    //std::string input_name = "ebl/ebl_40_3.txt";
    //std::string input_name = "lars/lars_70_1.txt";

    //std::string input_name = "ebl/ebl_30_2.txt";
    //std::string input_name = "lars/lars_30_2.txt";

    std::string input_name = "lars/lars_150_1.txt";

    /*Parametros de las hormigas*/
    int number_anthill = 1000;
    int number_ants = 50;
    int e = 30;

    /*Parametros de mejora de soluciones*/
    float limit_iteration = 10;
    int k = 10;

    /*Parametro de creacion de soluciones*/
    float alpha = 2.0;
    float beta = 5.0;
    bool random_first_article = true;
    
    /*Parametros de la actualizacion de feromona*/
    float vapor = 0.25;
    float c = 0.0005;
    float gamma = 15.0;
    float epsilon = 10.0;

    float max_pheromone = 10.0;
    float min_pheromone = 0.1;

    bool set_author_penalty = true;
    float base_penalty = 100.0;
    float constant_for_penalty = 4.0;

    /*Parametros de muestra de datos por pantalla*/
    bool show_solution_construction = false;
    bool show_solution_quality = false;
    bool show_ant_information = false;
    bool show_best_ant = false;
    bool show_solution_improvement = true;
    bool show_very_best_solution = true;
    
    /*Registro de soluciones*/
    bool write_result = false;
    bool write_improvement = false;
    bool write_convergence = false;

    std::string result_file_name = "experimet.csv"; 
    std::string result_improvement = "improvement.csv";
    std::string resutl_convergence = "convergence.csv";

    std::string aux_path =  "input/"+ input_name;
    const char * input_file = aux_path.c_str();
    const char * solution_test_file = "output/output_example.txt";

    /*Inicio de timer*/
    auto start = high_resolution_clock::now();

    Data data;
    data.read_input_file(input_file);
    
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

        //new_solution.show_solution();
    }

    if(validation_process)
    { 
        std::cout << "Validating solution" << std::endl;

        Solutions new_solution;
        new_solution.read_solution(solution_test_file);
        
        validator.articles_in_diferent_sessions(data, authors, new_solution.get_scheduling());
        validator.article_assignment(data,new_solution);
        validator.capacity_session(sessions,new_solution.get_scheduling());
        validator.capacity_topics(topics,new_solution.get_scheduling());
        validator.show_comments();

        validator.solution_benefit(articles,new_solution.get_scheduling());    
    }

    /*Archivo que guarda la mejor solucion encontrada en cada hormiguero*/
    std::ofstream improvementFile;
    std::string aux_improvement_path = "results/" + result_improvement; 
    const char * result_improvement_path = aux_improvement_path.c_str();
    improvementFile.open(result_improvement_path,std::ios::out | std::ios::app);
    improvementFile<<
    "anthill"<<","<<
    "time [s]"<<","<<
    "benefit"<<","<<
    "P articles"<<","<<
    "P topics"<<","<<
    "quality"<<std::endl;

    /*Archivo que guarda la mejora que se produce a lo largo de las iteraciones*/
    std::ofstream convergenceFile;
    std::string aux_convergence_path = "results/" + resutl_convergence; 
    const char * result_convergence_path = aux_convergence_path.c_str();
    convergenceFile.open(result_convergence_path,std::ios::out | std::ios::app);
    convergenceFile<<
    "anthill"<<","<<
    "time [s]"<<","<<
    "benefit"<<","<<
    "P articles"<<","<<
    "P topics"<<","<<
    "quality"<<std::endl;

    /*Algoritmo de hormigas*/
    int number_articles = articles.get_number_articles();
    Ants ants = Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,articles.get_similarity_matrix(),vapor,c,gamma,epsilon,e);
    
    std::vector<std::vector<std::vector<std::vector<int>>>> very_best_solution;
    float very_best_solution_quality = 0;
    float best_time_execution;

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
            /*
            int n_articles_parelel_session = validator.articles_in_diferent_sessions(data,authors,scheduling); 
            int n_max_article_day = validator.capacity_topics(topics,scheduling);
            float solution_quality = ants.calculate_quality_solution(solution_benefit,n_articles_parelel_session,n_max_article_day);
            */

            float n_articles_parelel_session = validator.articles_in_diferent_sessions(data,authors,scheduling); 
            float n_max_article_day = validator.capacity_topics_V2(topics,scheduling);
            

            if((n_articles_parelel_session>0) && set_author_penalty)
            {
                base_penalty = n_articles_parelel_session * constant_for_penalty;
                std::cout<<"Base penalty autors: "<<base_penalty<<std::endl;
                set_author_penalty = false;
            }
            
            float solution_quality = validator.quality_solution(solution_benefit,n_articles_parelel_session,n_max_article_day,base_penalty);

            ants.save_solution(scheduling,solution_quality);

            if(show_solution_quality)
            {
                std::cout<<"Solution benefit:               "<<solution_benefit<<std::endl;
                std::cout<<"Pair articles paralel session:  "<<n_articles_parelel_session<<std::endl;
                std::cout<<"Average over max topic:         "<<n_max_article_day<<std::endl;
                std::cout<<"Solution quality:               "<<solution_quality<<std::endl;
            }

            std::vector<std::vector<std::vector<std::vector<int>>>>().swap(scheduling);
        }

        /*Mejoramiento de la solucion*/
        int number_best_solutions = ants.get_number_best_solutions();
        //std::cout<<"total solutions: "<<number_best_solutions<<std::endl;
        std::vector<std::vector<std::vector<std::vector<int>>>> solution_to_improve;
    
        for(int iter_solution = 0; iter_solution<number_best_solutions; iter_solution++)
        {
            //solution_to_improve = ants.get_best_solution(iter_solution);
            float current_quality_solution = ants.get_best_quality_solution(iter_solution);
            Improvement * improve_method = new Improvement(ants.get_best_solution(iter_solution),current_quality_solution, gamma, epsilon, articles, topics, authors);

            /*Implementacion de movimiento*/
            int count_with_out_improve;
            if(improve_method->get_number_autor_conflicts() > 0)
            { 
                count_with_out_improve = 0;
                while((count_with_out_improve < limit_iteration) && (improve_method->get_number_autor_conflicts() > 0))
                {
                    //selecciono los articulo de uno de los autores con mas conflictos
                    int id_article_1 = improve_method->select_article_from_most_authors_conflicts(authors, k);
                    int id_article_2 = improve_method->select_article_same_day_diferent_block(id_article_1);
                    count_with_out_improve += improve_method->swap_articles_V2(id_article_1,id_article_2,articles,topics,authors);
                }
            }
            if(improve_method->get_number_topics_conflicts() > 0.0)
            {   
                count_with_out_improve = 0;
                while((count_with_out_improve < limit_iteration) && (improve_method->get_number_topics_conflicts() > 0.0))
                {
                    //selecciona el par de articulos para intercambiar que causan problemas de cantidad de topicos
                    int id_topic_problem;
                    std::vector<int> article_topic = improve_method->article_topics_problem_and_topic(topics);
                    int id_article_1 = article_topic[0];
                    id_topic_problem = article_topic[1];
                    //no tenga el topico de id_article_1_
                    int id_article_2 = improve_method->select_article_diferent_dayV2(topics, id_article_1, id_topic_problem);
                    count_with_out_improve += improve_method->swap_articles_V2(id_article_1,id_article_2 ,articles,topics,authors);
                }
            }

            count_with_out_improve = 0;
            while(count_with_out_improve < limit_iteration)
            {  
                //selecciono un articulo de la session con menos beneficio
                int id_article_1 = improve_method->get_article_worst_session();
                int id_article_2 = improve_method->select_article_in_diferent_session(id_article_1);
                count_with_out_improve += improve_method->swap_articles_V2(id_article_1,id_article_2 ,articles,topics,authors); 
            }    
         
            float n_articles_parelel_session = improve_method->get_number_autor_conflicts();
            float n_average_max_article_day = improve_method->get_number_topics_conflicts();
            float solution_quality = validator.quality_solution(improve_method->get_benefit_solution_improved(),n_articles_parelel_session,n_average_max_article_day,base_penalty);
            ants.update_best_solution_V2(improve_method->get_solution_improved(),solution_quality, iter_solution);
            
            delete improve_method;
        }
        std::vector<std::vector<std::vector<std::vector<int>>>>().swap(solution_to_improve); 

        int pos_best_solution_in_anthill = 0;
        if(limit_iteration>0 && number_best_solutions>1)
        {
            pos_best_solution_in_anthill = ants.get_best_solution_found();
        }

        best_solution = ants.get_best_solution(pos_best_solution_in_anthill); 
        best_solution_quality = ants.get_best_quality_solution(pos_best_solution_in_anthill);
       
        //hay que optimizar esto
        /*
        float aux_benefit = validator.solution_benefit(articles,best_solution);
        float aux_articles = validator.articles_in_diferent_sessions(data,authors,best_solution);
        float aux_capacity = validator.capacity_topics(topics,best_solution);
        */

        float aux_benefit = validator.solution_benefit(articles,best_solution);
        float aux_capacity = validator.capacity_topics_V2(topics,best_solution);
        float aux_articles = validator.articles_in_diferent_sessions(data,authors,best_solution);
        //int aux_capacity_validator = validator.capacity_topics(topics,best_solution);
        
        if(show_best_ant){
            std::cout<<"Best Ant"<<std::endl; 
            std::cout<<"Solution benefit:               "<<aux_benefit<<std::endl;
            std::cout<<"Pair articles paralel session:  "<<aux_articles<<std::endl;
            std::cout<<"Average over max topic:         "<<aux_capacity<<std::endl;
            std::cout<<"Solution quality:               "<<best_solution_quality<<std::endl;
        }

        /*actualizacion de la feromona*/
        ants.pheromone_update_list();
        ants.reset_ants();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start).count()/1000000.0;

        if(write_improvement)
        {
            //auto stop = high_resolution_clock::now();
            //auto duration = duration_cast<microseconds>(stop - start).count()/1000000.0;

            improvementFile<<
            anthill<<","<<
            std::setprecision(1) << std::fixed << duration<<","<<
            aux_benefit<<","<<
            aux_articles<<","<<
            aux_capacity<<","<<
            std::setprecision(1) << std::fixed <<best_solution_quality<<std::endl;
        }

        if(best_solution_quality>very_best_solution_quality)
        {
            very_best_solution_quality = best_solution_quality;
            very_best_solution = best_solution;
            best_time_execution = duration;

            if(write_convergence)
            {
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start).count()/1000000.0;

                convergenceFile<<
                anthill<<","<<
                std::setprecision(1) << std::fixed << duration<<","<<
                aux_benefit<<","<<
                aux_articles<<","<<
                aux_capacity<<","<<
                std::setprecision(1) << std::fixed << best_solution_quality<<std::endl;
            }

            if(show_solution_improvement)
            {
                std::cout<<"New best solution: "<<very_best_solution_quality<<std::endl;
            }

        }

        std::vector<std::vector<std::vector<std::vector<int>>>>().swap(best_solution);
    }

    /*Datos de la mejor solucion*/
    float very_best_solution_benefit = validator.solution_benefit(articles,very_best_solution);
    float n_articles_parelel_session = validator.articles_in_diferent_sessions(data,authors,very_best_solution); 
    float n_max_article_day = validator.capacity_topics_V2(topics,very_best_solution);

    if(show_very_best_solution)
    {
        std::cout<<"\nInformation very best solution"<<std::endl;
        std::cout<<"Solution benefit:               "<<very_best_solution_benefit<<std::endl;
        std::cout<<"Pair articles paralel session:  "<<n_articles_parelel_session<<std::endl;
        std::cout<<"Average over max topic:         "<<n_max_article_day<<std::endl;
        std::cout<<"Solution quality:               "<<very_best_solution_quality<<std::endl;
    }

    improvementFile.close();

    /*Registro de resultdos*/
    if(write_result)
    {
        std::ofstream resultsFile;
        std::string aux_result_path = "results/" + result_file_name; 
        const char * result_file_path = aux_result_path.c_str();
        resultsFile.open(result_file_path,std::ios::out | std::ios::app);

        auto stop = high_resolution_clock::now();
        auto delta_time = duration_cast<microseconds>(stop - start).count()/1000000.0;

        resultsFile << 
        input_name <<","<<
        number_anthill<<","<<
        number_ants<<","<<
        e <<","<<
        limit_iteration<<","<<
        alpha <<","<<
        beta <<","<<
        max_pheromone<<","<<
        min_pheromone<<","<<
        vapor <<","<<
        c <<","<<
        //gamma <<","<<
        //epsilon <<","<<
        std::setprecision(1) << std::fixed << delta_time <<","<<
        std::setprecision(1) << std::fixed <<best_time_execution <<","<<
        very_best_solution_benefit<<","<<
        n_articles_parelel_session<<","<<
        n_max_article_day<<","<<
        std::setprecision(1) << std::fixed << very_best_solution_quality<<std::endl;

        resultsFile.close();
    }

    convergenceFile.close();

    return 0;
}