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

int main(int argc,char* argv[]) {
    
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

    std::string input_name = "lars_modified/lars_modified_200_5.txt";
    unsigned int seed = 96;

    /*Parametros de las hormigas*/
    int number_anthill = 2;
    int number_ants = 2;
    int e = 1;
    float base_penalty = 100.0;
    float initial_quality = 200.0;
    float constant_for_penalty = 4.0;

    /*Parametros de la actualizacion de feromona*/
    float alpha = 2.0;
    float beta = 5.0;
    float vapor = 0.25;
    float c = 0.0001;
    float max_pheromone = 10.0;
    float min_pheromone = 0.1;
    int number_anthill_to_reset = 5; 

    /*Parametros de mejora de soluciones*/
    int limit_iteration = 50;
    int limit_benefit_search = 10;
    int k = 10;

    /*Toma de paramatros por bash*/
    if(argc>1)
    {
        input_name = argv[1];
        seed = std::stoi(argv[2]);

        number_anthill = std::stoi(argv[3]);
        number_ants = std::stoi(argv[4]);
        e = std::stoi(argv[5]);
        base_penalty = std::stof(argv[6]);
        initial_quality = std::stof(argv[6]);
        
        constant_for_penalty = std::stof(argv[7]);

        /*Parametros de la actualizacion de feromona*/
        alpha = std::stof(argv[8]);
        beta = std::stof(argv[9]);
        vapor = std::stof(argv[10]);
        c = std::stof(argv[11]);
        max_pheromone = std::stof(argv[12]);
        min_pheromone = std::stof(argv[13]);
        number_anthill_to_reset = std::stoi(argv[14]);

        /*Parametros de mejora de soluciones*/
        limit_iteration = std::stoi(argv[15]);
        limit_benefit_search = std::stoi(argv[16]);
        k = std::stoi(argv[17]);
    }

    std::cout<<input_name<<std::endl;
    //printf("limit_iteration: %d\n",limit_iteration);
    //printf("limit_benefit_search: %d\n",limit_benefit_search);
    //printf("valor c %f\n",c);
    //printf("limit iteration: %d\n",limit_iteration);
    
    /*Parametros de creacion de soluciones*/
    bool random_first_article = true;
    bool set_author_penalty = true;
    bool set_initial_quality = true;

    /*Parametros de muestra de datos por pantalla*/
    bool show_solution_construction = false;
    bool show_solution_quality = false;
    bool show_ant_information = false;
    bool show_best_ant = false;
    bool show_before_after_local_search = false;
    bool show_solution_improvement = true;
    bool show_schedule_detail = false;

    bool show_local_search_authors = false;
    bool show_local_search_topics = false;
    bool show_local_search_benefit = false;
    
    bool show_very_best_solution = false;
    
    /*Registro de soluciones*/
    bool write_result = true;
    bool write_improvement = false;
    bool write_convergence = false;

    std::string result_file_name = "experimet.csv";
    std::string result_improvement = "improvement.csv";
    std::string resutl_convergence = "convergence.csv";

    std::string aux_path =  "input/"+ input_name;
    const char * input_file = aux_path.c_str();
    
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
        const char * solution_test_file = "output/output_example.txt";
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
    if (write_improvement)
    {
        std::string aux_improvement_path = "results/" + result_improvement; 
        const char * result_improvement_path = aux_improvement_path.c_str();
        improvementFile.open(result_improvement_path,std::ios::out | std::ios::app);
        improvementFile<<
        "anthill"<<","<<
        "time [s]"<<","<<
        "mean p"<<","<<
        "std p"<<","<<
        "benefit"<<","<<
        "P articles"<<","<<
        "P topics"<<","<<
        "quality"<<std::endl;
    }
    
    /*Archivo que guarda la mejora que se produce a lo largo de las iteraciones*/
    std::ofstream convergenceFile;
    if (write_convergence)
    {

        std::string aux_convergence_path = "results/" + resutl_convergence; 
        const char * result_convergence_path = aux_convergence_path.c_str();
        convergenceFile.open(result_convergence_path,std::ios::out | std::ios::app);
        convergenceFile<<
        "anthill"<<","<<
        "time [s]"<<","<<
        "mean p"<<","<<
        "std p"<<","<<
        "benefit"<<","<<
        "P articles"<<","<<
        "P topics"<<","<<
        "quality"<<std::endl;
    }
    
    /*Algoritmo de hormigas*/
    int number_articles = articles.get_number_articles();
    Ants ants = Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,articles.get_similarity_matrix(),vapor,c,e,seed);
    
    
    std::vector<std::vector<std::vector<std::vector<int>>>> very_best_solution;
    float very_best_solution_quality = 0;
    long double best_time_execution;

    //std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(seed); // seed the generator
    std::uniform_int_distribution<> distr(0, number_articles-1); // define the range
    srand(seed);

    if(show_solution_improvement)
    {
        printf("Anthill\tTime\tMean p\tStd p\tBene\tAuthors\tTopics\tQuality\n");
    }

    int anthill=0;
    int limit_iterations_without_improvement  = 1000;
    int current_iterarion_without_improvement = 0;

    while((anthill<number_anthill) && (current_iterarion_without_improvement < limit_iterations_without_improvement))

    //for(int int anthill=0; anthill<number_anthill;anthill++)
    {   
        //std::cout<<"Conjunto de hormigas: "<<anthill<<std::endl;
        
        std::vector<std::vector<std::vector<std::vector<int>>>> best_solution;
        float best_solution_quality = -1;

        std::vector<int> available_articles; 
        std::vector<int> solution_articles;

        ants.reset_pheromone(anthill, number_anthill_to_reset);

        if(show_solution_quality)
        {
            printf("\nSoluciones de la iteracion: %d\n",anthill);
        }  

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
                //printf("articlulo incial %d\n",initial_article);  
            }
            
            int id_inital_article = available_articles[initial_article];
            //ants.save_solution(id_ant,id_inital_article);
            solution_articles.push_back(id_inital_article);
            available_articles.erase(available_articles.begin()+initial_article);

            while(available_articles.size()>0)
            {
                int next_article = ants.get_next_article(id_inital_article,available_articles,rand());
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
            float n_max_article_day = validator.capacity_topics_V2(topics,scheduling);
            
            if((n_articles_parelel_session>0) && set_author_penalty)
            {
                base_penalty = n_articles_parelel_session * constant_for_penalty;
                //std::cout<<"Base penalty autors: "<<base_penalty<<std::endl;
                set_author_penalty = false;
            }
            
            float solution_quality = validator.quality_solution(solution_benefit,n_articles_parelel_session,n_max_article_day,base_penalty);

            ants.save_solution(scheduling,solution_benefit,n_articles_parelel_session,n_max_article_day,solution_quality,id_ant);

            if(show_solution_quality)
            {   
                /*
                std::cout<<"Solution benefit:               "<<solution_benefit<<std::endl;
                std::cout<<"Pair articles paralel session:  "<<n_articles_parelel_session<<std::endl;
                std::cout<<"Average over max topic:         "<<n_max_article_day<<std::endl;
                std::cout<<"Solution quality:               "<<solution_quality<<std::endl;
                */

                printf("%d\t%d\t%d\t%d\t%f\t%f\n",anthill,id_ant,solution_benefit,n_articles_parelel_session,n_max_article_day,solution_quality);
            }

            std::vector<std::vector<std::vector<std::vector<int>>>>().swap(scheduling);
        }

        /*Mejoramiento de la solucion*/
        if(show_before_after_local_search)
        {
            printf("Mejores Soluciones antes de la busqueda local\n");
            ants.show_best_ants(anthill);
        }

        int number_best_solutions = ants.get_number_best_solutions();
        //std::vector<std::vector<std::vector<std::vector<int>>>> solution_to_improve;
    
        /*Implementacion de movimiento*/
        for(int iter_solution = 0; iter_solution<number_best_solutions; iter_solution++)
        {
            float current_quality_solution = ants.get_best_quality_solution(iter_solution);
            Improvement * improve_method = new Improvement(ants.get_best_solution(iter_solution),current_quality_solution, articles, topics, authors,seed);

            int count_with_out_improve;

            if(improve_method->get_number_autor_conflicts() > 0)
            { 
                if(show_local_search_authors)
                {
                    printf("movimiento de topes de horario, solucion %d\n", iter_solution);
                }
                
                count_with_out_improve = 0;
                while((count_with_out_improve < limit_iteration) && (improve_method->get_number_autor_conflicts() > 0))
                {
                    //selecciono los articulo de uno de los autores con mas conflictos
                    int id_article_1 = improve_method->select_article_from_most_authors_conflicts(authors, k, rand());
                    //printf("id_article_1 %d\n",id_article_1);
                    int id_article_2 = improve_method->select_article_same_day_diferent_block(id_article_1,rand());
                    //printf("id_article_2 %d\n",id_article_2);
                    count_with_out_improve += improve_method->swap_articles_V2(id_article_1,id_article_2,articles,topics,authors,show_local_search_authors);
                }
            }

            if(improve_method->get_number_topics_conflicts() > 0.0)
            {   
                if(show_local_search_topics)
                {
                    printf("movimiento de topicos, solucion %d\n", iter_solution);
                }
                
                count_with_out_improve = 0;
                while((count_with_out_improve < limit_iteration) && (improve_method->get_number_topics_conflicts() > 0.0))
                {
                    //selecciona el par de articulos para intercambiar que causan problemas de cantidad de topicos
                    std::vector<int> article_topic = improve_method->article_topics_problem_and_topic(topics, rand());
                    int id_article_1 = article_topic[0];
                    int id_topic_problem = article_topic[1];
                    //no tenga el topico de id_article_1_
                    int id_article_2 = improve_method->select_article_diferent_dayV2(topics, id_article_1, id_topic_problem,rand());
                    //printf("%d\t%d\n",id_article_1,id_article_2);
                    count_with_out_improve += improve_method->swap_articles_V2(id_article_1,id_article_2 ,articles,topics,authors,show_local_search_topics);
                }
            }

            if(show_schedule_detail)
            {
                printf("\nSolucion antes del movimiento de mejora de beneficio\n");
                validator.show_schedule_information(improve_method->get_solution_improved(),topics,authors,articles);
            }

            if(show_local_search_benefit)
            {
                printf("movimiento de mejora de beneficio, solucion %d\n", iter_solution);
            }

            
            int count_number_pair_sessions = 0;
            while(count_number_pair_sessions < limit_benefit_search)
            {
                std::vector<std::vector<int>> list_sessions_by_ponderation = improve_method->get_list_session_sorted_by_ponderation();
                //printf("number_sessions: %ld\n",list_sessions_by_ponderation.size());

                std::mt19937 gen(rand());
                std::uniform_int_distribution<> distr_sessions(0, (int) (0.5*list_sessions_by_ponderation.size()) - 1);
                int index_session_1 = distr_sessions(gen);
                std::vector<int> session_1 = list_sessions_by_ponderation[index_session_1];

                //printf("selected session 1\n");
                std::vector<int> articles_in_session_1 = improve_method->get_articles_session(session_1);
                int number_articles_session_1 = articles_in_session_1.size();
                //printf("Session_1 selected\n");
                //printf("%d\t%d\t%d\n",session_1[0],session_1[1],session_1[2]);

                list_sessions_by_ponderation.erase(list_sessions_by_ponderation.begin() + index_session_1);
                //printf("number session left %ld\n",list_sessions_by_ponderation.size());

                for(auto session_2:list_sessions_by_ponderation)
                {
                    //printf("selected session 2\n");
                    std::vector<int> articles_in_session_2 = improve_method->get_articles_session(session_2);
                    int number_articles_session_2 = articles_in_session_2.size();

                    for(int iter_article_1 = 0; iter_article_1 < number_articles_session_1; iter_article_1++)
                    {
                        int id_article_1 = articles_in_session_1[iter_article_1];

                        for(int iter_article_2 = 0; iter_article_2 < number_articles_session_2; iter_article_2++)
                        {
                            int id_article_2 = articles_in_session_2[iter_article_2];
                            int case_improvement = improve_method->swap_articles_V2(id_article_1,id_article_2 ,articles,topics,authors,show_local_search_benefit);
                        
                            if(case_improvement == 0)
                            {
                                articles_in_session_1 = improve_method->get_articles_session(session_1);
                                articles_in_session_2 = improve_method->get_articles_session(session_2);

                                id_article_1 = articles_in_session_1[iter_article_1];
                            }
                        }   
                    }

                    std::vector<int>().swap(articles_in_session_2);
                    count_number_pair_sessions += 1;
                }

                std::vector<int>().swap(session_1);
                std::vector<int>().swap(articles_in_session_1);
                std::vector<std::vector<int>>().swap(list_sessions_by_ponderation);

            }

            
            int benefit_improved = improve_method->get_benefit_solution_improved();
            int n_articles_parelel_session_improved = improve_method->get_number_autor_conflicts();
            float n_average_max_article_day_improved = improve_method->get_number_topics_conflicts();

            /*
            int benefit_check = validator.solution_benefit(articles,improve_method->get_solution_improved());
            int n_articles_parelel_session_check = validator.articles_in_diferent_sessions(data,authors,improve_method->get_solution_improved()); 
            float n_max_article_day_check = validator.capacity_topics_V2(topics,improve_method->get_solution_improved());
        
            printf("\nresultado mejora\n");
            printf("%d\t%d\t%f\n", benefit_improved, n_articles_parelel_session_improved, n_average_max_article_day_improved);
            printf("%d\t%d\t%f\n\n",  benefit_check, n_articles_parelel_session_check, n_max_article_day_check);
            */

            float solution_quality_improved = validator.quality_solution(benefit_improved,n_articles_parelel_session_improved,n_average_max_article_day_improved,base_penalty);
            
            if((solution_quality_improved > 0) && set_initial_quality)
            {
                initial_quality = solution_quality_improved;
                set_initial_quality = false;
                //printf("initial quality:\t%f",initial_quality);
            }
            
            ants.update_best_solution_V2(improve_method->get_solution_improved(),benefit_improved, n_articles_parelel_session_improved,n_average_max_article_day_improved,solution_quality_improved, iter_solution);
            ants.save_solution_authors_problems(improve_method->get_article_ubication(),iter_solution, authors);

            if(show_schedule_detail)
            {
                validator.show_schedule_information(improve_method->get_solution_improved(),topics,authors,articles);
            }
            
            delete improve_method;
        }
        //std::vector<std::vector<std::vector<std::vector<int>>>>().swap(solution_to_improve); 

        int pos_best_solution_in_anthill = 0;
        if(limit_iteration>0 && number_best_solutions>1)
        {
            pos_best_solution_in_anthill = ants.get_best_solution_found();
        }

        best_solution = ants.get_best_solution(pos_best_solution_in_anthill); 
        best_solution_quality = ants.get_best_quality_solution(pos_best_solution_in_anthill);
       
        int aux_benefit = ants.get_best_benefit_solution(pos_best_solution_in_anthill);
        int aux_articles = ants.get_best_authors_problems_solution(pos_best_solution_in_anthill);
        float aux_capacity = ants.get_best_topics_problems_solution(pos_best_solution_in_anthill);

        //hay que optimizar esto
        /*
        int aux_benefit = validator.solution_benefit(articles,best_solution);
        float aux_articles = validator.articles_in_diferent_sessions(data,authors,best_solution);
        float aux_capacity = validator.capacity_topics_V2(topics,best_solution);
        */

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start).count()/1000000.0;

        if(show_before_after_local_search)
        {
            printf("Mejores Soluciones despues de la busqueda local\n");
            ants.show_best_ants(anthill);
        }

        /*actualizacion de la feromona*/
        ants.pheromone_update_list(very_best_solution,very_best_solution_quality,anthill,number_anthill_to_reset,initial_quality);
        ants.reset_ants();
        
        if(show_best_ant){
            /*
            std::cout<<"Best Ant"<<std::endl; 
            std::cout<<"Solution benefit:               "<<aux_benefit<<std::endl;
            std::cout<<"Pair articles paralel session:  "<<aux_articles<<std::endl;
            std::cout<<"Average over max topic:         "<<aux_capacity<<std::endl;
            std::cout<<"Solution quality:               "<<best_solution_quality<<std::endl;
            std::cout<<std::endl;
            */

            std::vector<float> pheromone_matrix_indicator = ants.get_mean_and_des_std();

            std::cout<<
            anthill<<","<<
            std::setprecision(1) << std::fixed << duration<<","<<
            pheromone_matrix_indicator[0] <<","<<
            pheromone_matrix_indicator[1] <<","<<
            aux_benefit<<","<<
            aux_articles<<","<<
            aux_capacity<<","<<
            std::setprecision(1) << std::fixed <<best_solution_quality<<std::endl;

            std::vector<float>().swap(pheromone_matrix_indicator);
        }

        if(write_improvement)
        {
            //auto stop = high_resolution_clock::now();
            //auto duration = duration_cast<microseconds>(stop - start).count()/1000000.0;
            std::vector<float> pheromone_matrix_indicator = ants.get_mean_and_des_std();

            improvementFile<<
            anthill<<","<<
            std::setprecision(1) << std::fixed << duration<<","<<
            pheromone_matrix_indicator[0] <<","<<
            pheromone_matrix_indicator[1] <<","<<
            aux_benefit<<","<<
            aux_articles<<","<<
            aux_capacity<<","<<
            std::setprecision(1) << std::fixed <<best_solution_quality<<std::endl;

            std::vector<float>().swap(pheromone_matrix_indicator);
        }

        if(best_solution_quality>very_best_solution_quality)
        {
            very_best_solution_quality = best_solution_quality;
            very_best_solution = best_solution;
            best_time_execution = duration;

            current_iterarion_without_improvement = 0;

            if(write_convergence)
            {
                //auto stop = high_resolution_clock::now();
                //auto duration = duration_cast<microseconds>(stop - start).count()/1000000.0;
                std::vector<float> pheromone_matrix_indicator = ants.get_mean_and_des_std();

                convergenceFile<<
                anthill<<","<<
                std::setprecision(1) << std::fixed << duration<<","<<
                pheromone_matrix_indicator[0] <<","<<
                pheromone_matrix_indicator[1] <<","<<
                aux_benefit<<","<<
                aux_articles<<","<<
                aux_capacity<<","<<
                std::setprecision(1) << std::fixed << very_best_solution_quality<<std::endl;

                std::vector<float>().swap(pheromone_matrix_indicator);
            }

            if(show_solution_improvement)
            {
                /*
                std::cout<<"Solution benefit:               "<<aux_benefit<<std::endl;
                std::cout<<"Pair articles paralel session:  "<<aux_articles<<std::endl;
                std::cout<<"Average over max topic:         "<<aux_capacity<<std::endl;
                std::cout<<"Solution quality:               "<<very_best_solution_quality<<std::endl;
                std::cout<<std::endl;
                */
               
                std::vector<float> pheromone_matrix_indicator = ants.get_mean_and_des_std();

                std::cout<<
                anthill<<"\t\t"<<
                std::setprecision(1) << std::fixed << duration<<"\t\t"<<
                pheromone_matrix_indicator[0] <<"\t\t"<<
                pheromone_matrix_indicator[1] <<"\t\t"<<
                aux_benefit<<"\t\t"<<
                aux_articles<<"\t\t"<<
                std::setprecision(4) << std::fixed << aux_capacity<<"\t\t"<<
                std::setprecision(1) << std::fixed << very_best_solution_quality<<std::endl;

                std::vector<float>().swap(pheromone_matrix_indicator);
            }

        }

        else
        {
            current_iterarion_without_improvement ++;
            //printf("Iteration without improvemnt:\t%d\n",current_iterarion_without_improvement);
        }

        std::vector<std::vector<std::vector<std::vector<int>>>>().swap(best_solution);

        anthill ++;
    }

    /*Datos de la mejor solucion*/
    int very_best_solution_benefit = validator.solution_benefit(articles,very_best_solution);
    int n_articles_parelel_session = validator.articles_in_diferent_sessions(data,authors,very_best_solution); 
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
    auto stop = high_resolution_clock::now();
    auto delta_time = duration_cast<microseconds>(stop - start).count()/1000000.0;
    std::vector<float> pheromone_matrix_indicator = ants.get_mean_and_des_std();
    
    if(write_result)
    {
        std::ofstream resultsFile;
        std::string aux_result_path = "results/" + result_file_name; 
        const char * result_file_path = aux_result_path.c_str();
        resultsFile.open(result_file_path,std::ios::out | std::ios::app);

        /*
        auto stop = high_resolution_clock::now();
        auto delta_time = duration_cast<microseconds>(stop - start).count()/1000000.0;
        std::vector<float> pheromone_matrix_indicator = ants.get_mean_and_des_std();
        */

        resultsFile << 
        input_name <<","<<
        number_anthill<<","<<
        number_ants<<","<<
        e <<","<<
        seed <<","<<
        limit_iteration<<","<<
        limit_benefit_search<<","<<
        k<<","<<
        number_anthill_to_reset<<","<<
        alpha <<","<<
        beta <<","<<
        max_pheromone<<","<<
        min_pheromone<<","<<
        vapor <<","<<
        c <<","<<
        pheromone_matrix_indicator[0] <<","<<
        pheromone_matrix_indicator[1] <<","<<
        std::setprecision(1) << std::fixed << delta_time <<","<<
        std::setprecision(1) << std::fixed << best_time_execution <<","<<
        very_best_solution_benefit<<","<<
        n_articles_parelel_session<<","<<
        n_max_article_day<<","<<
        std::setprecision(1) << std::fixed << very_best_solution_quality<<std::endl;

        resultsFile.close();
    }

    convergenceFile.close();

    std::cout<<
        anthill<<"\t\t"<<
        std::setprecision(1) << std::fixed <<  delta_time <<"\t\t"<<
        std::setprecision(1) << std::fixed <<  pheromone_matrix_indicator[0] <<"\t\t"<<
        std::setprecision(1) << std::fixed <<  pheromone_matrix_indicator[1] <<"\t\t"<<
        very_best_solution_benefit<<"\t\t"<<
        n_articles_parelel_session<<"\t\t"<<
        std::setprecision(4) << std::fixed <<  n_max_article_day<<"\t\t"<<
        std::setprecision(1) << std::fixed <<  very_best_solution_quality
    <<std::endl;

    std::cout<<
        input_name <<","<<
        number_anthill<<","<<
        number_ants<<","<<
        e <<","<<
        seed <<","<<
        limit_iteration<<","<<
        limit_benefit_search<<","<<
        k<<","<<
        number_anthill_to_reset<<","<<
        alpha <<","<<
        beta <<","<<
        max_pheromone<<","<<
        min_pheromone<<","<<
        vapor <<","<<
        std::setprecision(6) << c <<","<<
        std::setprecision(1) << std::fixed << pheromone_matrix_indicator[0] <<","<<
        std::setprecision(1) << std::fixed << pheromone_matrix_indicator[1] <<","<<
        std::setprecision(1) << std::fixed << delta_time <<","<<
        std::setprecision(1) << std::fixed << best_time_execution <<","<<
        very_best_solution_benefit<<","<<
        n_articles_parelel_session<<","<<
        std::setprecision(4) << std::fixed << n_max_article_day<<","<<
        std::setprecision(1) << std::fixed << very_best_solution_quality
    <<std::endl;   

    printf("%f\n",very_best_solution_quality);

    return very_best_solution_quality;
}
