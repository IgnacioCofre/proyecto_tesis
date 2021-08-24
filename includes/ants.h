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

    public:
        //Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,Lk_constant,similarity_matrix)
        Ants(int,float,float,int, float, float, float,std::vector<std::vector<int>>); 
        int get_number_ants(void);

        //get_next_article(i,list_j,similarity_matrix)
        int get_next_article(int, std::vector<int>); 

        float get_test(int, int, int);
};

#endif