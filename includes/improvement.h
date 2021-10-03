#ifndef IMPROVEMENT_H
#define IMPROVEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include "articles.h"
#include "topics.h"
#include "authors.h"
#include "random"

class Improvement
{
    std::vector<std::vector<std::vector<std::vector<int>>>> solution_to_improve;
    //article_ubication[id_article] = [day,block,room,position_in_list] of article assignation in solution_to_improve
    std::vector<std::vector<int>> article_ubication;

    //benefit_session[day,block,room] = int benefit of session
    std::vector<std::vector<std::vector<int>>> benefit_session; 

    //authors_conflicts[id_author] = int number of conflicts tope de horario por autor
    std::vector<int> authors_conflicts;
    
    //number_article_per_day_by_topic[day][id_topic] = int number of article in the day that contains id_topic
    std::vector<std::vector<int>> number_article_per_day_by_topic;
    
    int total_benefit= 0;
    int number_autor_conflicts = 0;
    float number_topics_conflics = 0.0;
    float current_solution_quality;
    float gamma, epsilon;
    float limit_iteration;

    public:
        //Improvement(solution, quality, gamma, epsilon, articles, topics, authors)
        Improvement(std::vector<std::vector<std::vector<std::vector<int>>>>, float, float, float,Articles, Topics, Authors);
        
        /*
        swap_articles(id_article_1, id_article_2)
        Se realiza un intercambio de los articulos id_1 y id_2, donde se evalua si dicho 
        intercambio produce alguna mejora sobre la solucion solution_to_improve
        */
        void swap_articles(int, int,  Articles, Topics, Authors);
        void swap_articles_V2(int, int, Articles, Topics, Authors);

        /*
        show_data()
        muestra la informacion de:
        article_ubication
        benefit_session 
        authors_conflicts_per_block
        number_article_per_day_by_topic
        */
        void show_data(void);
        
        /*
        in_diferent_session(id_article_1,id_article_2) = bool 
        True si los articulos estan en distintas sesiones
        False si los articulos estan en la misma sesion
        */
        bool in_diferent_session(int,int);

        /*
        select_aricle_in_diferent_session(id_article) = int del proximo id_article a cambiar
        Retorna el id de un article que no se encuentre en la misma sesion que id_article
        */
        int select_article_in_diferent_session(int);

        /*
        get_quality_parameters() = {total_benefit,number_autor_conflicts,number_topics_conflics} 
        Retorna los valores necesarios para calcular la calidad de la solucion 
        */
        //std::vector<int> get_quality_parameters(void);

        /*
        get_solution_improved() = solution_to_improve
        Retorna solution solution_to_improve
        */
        std::vector<std::vector<std::vector<std::vector<int>>>> get_solution_improved(void);

        /*
        get_benefit_solution_improved()
        Retorna el beneficio de la solucion mejorada total_benefit
        */
        int get_benefit_solution_improved(void);

        /*
        get_number_autor_conflicts() = number_autor_conflicts
        Retorna el numero de conflictos por topes de horario de
        la solucion actual
        */
        int get_number_autor_conflicts(void);

        /*
        get_number_topics_conflicts
        */
        float get_number_topics_conflicts(void);


};

#endif
