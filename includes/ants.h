#ifndef ANTS_H
#define ANTS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "improvement.h"

class Ants
{
    int number_ants, number_articles;
    float max_pheromone, min_pheromone;
    //alpha: importancia empírica
    //beta: importancia heuristica
    float alpha, beta;
    float c_factor, vapor_factor;
    //float  gamma_factor, epsilon_factor; 
    unsigned int seed;
    int e;
    std::vector<std::vector<float>> pheromone_matrix;
    //ant_solution_scheduling[id_ant][day][block][room] = [id articles in session]
    std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> ant_solution_scheduling;

    //solution_ant[id_ant] = float solution_quality
    std::vector<float> solution_ant;
    std::vector<int> benefit_ant;
    std::vector<int> number_authors_problems_ant;
    std::vector<float> topics_problems_ant;
    std::vector<int> id_ants_solutions;
    
    std::vector<std::vector<int>> similarity_matrix;

    //solution_ant[id_ant] = [ids_articles]
    //std::vector<std::vector<int>> solution_ant;

    public:
        //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,articles.get_similarity_matrix(),vapor_factor, c_factor,e_max,seed)
        Ants(int,float,float,int, float, float,std::vector<std::vector<int>>, float, float,int,unsigned int); 
        int get_number_ants(void);

        /*
        get_next_article(id_article,list_id_articles, internal_seed)
        Retorna el id del siguiente articulo seleccionado para ser parte de la solucion
        */
        int get_next_article(int, std::vector<int>, int); 

        /*
        create_solution(id_ant,max_assign_per_session,[id_articles])
        Crea la solucion que se va contrullendo la solucion en ant_solution_scheduling[id_ant]
        en base al formato de max_assign_per_session
        */
        std::vector<std::vector<std::vector<std::vector<int>>>> create_solution(int,std::vector<std::vector<std::vector<int>>>, std::vector<int>);

        /*
        save_solution(schedule, solution_benefit,n_articles_parelel_session,n_max_article_day,solution_quality,id_ant_to_save)
        Guarda las mejores e soluciones en la lista ant_solution_scheduling y solution_ant
        */
        void save_solution(std::vector<std::vector<std::vector<std::vector<int>>>>, int, int, float, float,int);

        /*
        reset_ants()
        Se borra el contenido de ant_solution_scheduling y solution_ant
        */
        void reset_ants(void);

        float get_test(int, int);

        /*
        pheromone_update(scheduling, solution_quality)
        Acutaliza el valor de los arcos en la matriz de feromonas
        */
        void pheromone_update(std::vector<std::vector<std::vector<std::vector<int>>>>, float);

        /*
        Calucla la calidad final de la solucion
        solution_quality = c*(solution_benefit - gamma*autor_problems - epsilon*topics_problems)
        */
        //float calculate_quality_solution(int solution_benefit, int autor_problems, int topcis_problems);

        /*
        pheromone_update_list()
        Actualiza el valor de los arcos de la matriz de feromona 
        a partir de los valores de solution_ant y las calendarizaciones de ant_solution_scheduling
        */
        void pheromone_update_list(void); 

        /*
        Retorna la cantidad de soluciones que se han guardado en la lista de mejores 
        soluciones ant_solution_scheduling
        */
        int get_number_best_solutions(void);

        /*
        Retorna la mejor solucion de ant_solution_scheduling, donde 0 es la mejor solucion
        */
        std::vector<std::vector<std::vector<std::vector<int>>>> get_best_solution(int);

        /*
        Retorna la calidad de la lista de mejores solucion de solution_ant, donde 0 es la mejor solucion
        */
        float get_best_quality_solution(int);

        int get_best_benefit_solution(int);

        int get_best_authors_problems_solution(int);

        float get_best_topics_problems_solution(int);
        
        /*
        solution_quality(id_ant,max_assign_per_session)
        calcula el beneficio total de la solucion creada por la hormiga id_ant
        */
        //int solution_quality_v1(int,std::vector<std::vector<std::vector<int>>>);

        /*
        solution_quality(id_ant,max_assign_per_session)
        calcula el beneficio total de la solucion creada por la hormiga id_ant
        V2 se cambia el orden de instanciacion
        */
        //int solution_quality_v2(int,std::vector<std::vector<int>>);

        /*
        update_best_solution(solution,solution_quality,id_ant)
        Actualiza la solucion mejorada en el objeto Improvement de acuerdo a la lista de mejores soluciones 
        ant_solution_scheduling[id_ant] = new_solution
        solution_ant[id_ant] = new_quality
        */
        void update_best_solution(Improvement,float,int);
        
        /*
        get_best_solution_found() = int posicion de la mejor solucion 
        Retorna la posicion de la mejor solucion a partir de la calidad encontrada en las soluciones 
        de solution_ant
        */
        int get_best_solution_found(void);

        /*
        update_best_solution_v2(schedule,benefit, n_problemas_autores, n_problemas_topicos,quality,id_ant)
        Actualiza la solucion de la hormiga id_ant con el schedule solo si la quality es mejor que la anteriormente 
        guardada en id_ant
        */    
        void update_best_solution_V2(std::vector<std::vector<std::vector<std::vector<int>>>>,int,int,float,float,int);

        /*
        std::vector<float> get_mean_and_des_std() = [mean,std]
        entrega la desviacion estandar de la matriz de feromonas actual
        */
        std::vector<float> get_mean_and_des_std(void);

        /*
        void reset_pheromone(actua_iteration, number_iteration_to_reset);  
        Resetea la matriz de feromona al max_pheromone. 
        */       
        void reset_pheromone(int, int);

        /*
        void show_best_ants(anthill)
        Muestra por pantalla las mejores hormigas encontradas
        */
        void show_best_ants(int);
        
};

#endif