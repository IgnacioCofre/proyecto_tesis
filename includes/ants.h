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
    float Lk_constant;
    std::vector<std::vector<float>> pheromone_matrix;
    std::vector<std::vector<std::vector<float>>> ants_pheromone_matrix;

    std::vector<std::vector<int>> similarity_matrix;

    //solution_ant[id_ant] = [ids_articles]
    std::vector<std::vector<int>> solution_ant;

    public:
        //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant,similarity_matrix)
        Ants(int,float,float,int, float, float, float,std::vector<std::vector<int>>); 
        int get_number_ants(void);

        /*
        get_next_article(id_article,list_id_articles)
        Retorna el id del siguiente articulo seleccionado para ser parte de la solucion
        */
        int get_next_article(int, std::vector<int>); 

        /*
        save_solution(id_ant,id_article)
        Guarda la solucion que se va contrullendo en solution_ant[id_ant]
        */
        int save_solution(int,int);

        /*
        get_solution_ant(id_ant)
        Retorna la lista de solucion en forma de una lista de ids de articulos
        */
        std::vector<int> get_solution_ant(int);

        void  reset_ants(void);

        float get_test(int, int, int);

        /*
        solution_quality(id_ant,max_assign_per_session)
        calcula el beneficio total de la solucion creada por la hormiga id_ant
        */
        int solution_quality_v1(int,std::vector<std::vector<std::vector<int>>>);

        /*
        solution_quality(id_ant,max_assign_per_session)
        calcula el beneficio total de la solucion creada por la hormiga id_ant
        V2 se cambia el orden de instanciacion
        */
        int solution_quality_v2(int,std::vector<std::vector<int>>);
};

#endif