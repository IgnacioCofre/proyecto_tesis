#ifndef IMPROVEMENT_H
#define IMPROVEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include "articles.h"
#include "topics.h"
#include "authors.h"

class Improvement
{
    std::vector<std::vector<std::vector<std::vector<int>>>> solution_to_improve;
    //article_ubication[id_article] = [day,block,room] of article assignation in solution_to_improve
    std::vector<std::vector<int>> article_ubication;

    //benefit_session[day,block,room] = int benefit of session
    std::vector<std::vector<std::vector<int>>> benefit_session; 

    //authors_conflicts_per_block[day][block] = int number of conflicts tope de horario 
    std::vector<std::vector<int>> authors_conflicts_per_block;
    
    //number_article_per_day_by_topic[day][id_topic] = int number of article in the day that contains id_topic
    std::vector<std::vector<int>> number_article_per_day_by_topic;

    int number_autor_conflicts, number_topics_conflics;
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
        void swap_articles(int, int, Articles);

        /*
        show_data()
        muestra la informacion de:
        article_ubication
        benefit_session 
        authors_conflicts_per_block
        number_article_per_day_by_topic
        */
        void show_data(void);

};

#endif
