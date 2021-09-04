#ifndef ANTS_H
#define ANTS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>

class Ants
{
    int number_ants, number_articles;
    float max_pheromone, min_pheromone;
    //alpha: importancia empírica
    //beta: importancia heuristica
    float alpha, beta;
    float c_factor, vapor_factor, gamma_factor, epsilon_factor ;
    std::vector<std::vector<float>> pheromone_matrix;
    //ant_solution_scheduling[id_ant][day][block][room] = [id articles in session]
    //std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> ant_solution_scheduling;

    std::vector<std::vector<int>> similarity_matrix;

    //solution_ant[id_ant] = [ids_articles]
    //std::vector<std::vector<int>> solution_ant;

    public:
        //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,articles.get_similarity_matrix(),vapor_factor, c_factor,gamma_factor,epsilon_factor)
        Ants(int,float,float,int, float, float,std::vector<std::vector<int>>, float, float, float, float); 
        int get_number_ants(void);

        /*
        get_next_article(id_article,list_id_articles)
        Retorna el id del siguiente articulo seleccionado para ser parte de la solucion
        */
        int get_next_article(int, std::vector<int>); 

        /*
        save_solution(id_ant,max_assign_per_session,[id_articles])
        Crea y guarda la solucion que se va contrullendo la solucion en ant_solution_scheduling[id_ant]
        en base al formato de max_assign_per_session
        */
        std::vector<std::vector<std::vector<std::vector<int>>>> save_solution(int,std::vector<std::vector<std::vector<int>>>, std::vector<int>);

        /*
        get_solution_ant(id_ant)
        Retorna la calendarizacion resultante contruida por la hormiga id_ant
        */
        //std::vector<std::vector<std::vector<std::vector<int>>>> get_solution_ant(int);

        //void  reset_ants(void);

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
        float calculate_quality_solution(int solution_benefit, int autor_problems, int topcis_problems);


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

        
};

#endif