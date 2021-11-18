#ifndef IMPROVEMENT_H
#define IMPROVEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
    unsigned int seed;
    float number_topics_conflics = 0.0;
    float current_solution_quality;
    float limit_iteration;

    //std::random_device rd;

    public:
        //Improvement(solution, quality, articles, topics, authors,seed)
        Improvement(std::vector<std::vector<std::vector<std::vector<int>>>>, float,Articles, Topics, Authors, unsigned int);
        
        /*
        swap_articles(id_article_1, id_article_2, articles, topics, authors, show_improvement) = 1: no hay mejora, 0: si hay mejora
        Se realiza un intercambio de los articulos id_1 y id_2, donde se evalua si dicho 
        intercambio produce alguna mejora sobre la solucion solution_to_improve
        */
        void swap_articles(int, int,  Articles, Topics, Authors);
        int swap_articles_V2(int, int, Articles, Topics, Authors, bool);

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
        select_aricle_in_diferent_session(id_article,internal_seed) = int del proximo id_article a cambiar
        Retorna el id de un article que no se encuentre en la misma sesion que id_article
        */
        int select_article_in_diferent_session(int,int);

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
        retorna la media de problemas de topicos
        */
        float get_number_topics_conflicts(void);

        /*
        get_articles_autor_conflicts(class Authors) = [id_articles]
        retorna los articulos del autor con mas problemas con topes de horario
        */
        std::vector<int> get_articles_author_conflicts(Authors);

        /*
        get_articles_worst_session(internal_seed) = [id_articles]
        Retorna los id de los articulos pertenecientes a la session con menor beneficio 
        a partir de la lista benefit_session
        */
        int get_article_worst_session(int);

        /*
        select_article_in_diferent_block(id_article_1) = id_article_2
        Retorna un id de un articulo que este en un bloque distinto al id_article_2
        asignado en la solucion
        */
        int select_article_in_diferent_block(int);

        /*
        select_article_most_authors_conflicts(void) = id_article
        Retorna el id del articulo que causa mayores conflictos de topes de horario
        del autor con mas conflictos de topes de horario
        */
        int select_article_most_authors_conflicts(Authors);

        /*
        select_article_from_most_authors_conflicts(authors, K,internal_seed) = id_article
        Retorna un id articulo del conjunto de los autores con mas topes de horario
        */
        //int select_article_from_most_authors_conflicts(Authors, int, std::mt19937);
        int select_article_from_most_authors_conflicts(Authors, int,int);

        /*
        select_article_same_day_diferent_block(id_article_1,internal_seed)
        Retorna el id de un articulo que se encuentre dentro del mismo dia pero en
        un bloque distinto que el id_article_1
        */
        int select_article_same_day_diferent_block(int, int);

        /*
        int article_topics_problem(topics) = id_article_1
        Retorna un id del articulo que causa problemas con la restriccion de maximo de articulos por topico
        */
        int article_topics_problem(Topics);

        /*
        std::vector<int> article_topics_problem_and_topic(Topics, internal_seed) = [id_article, id_topic]
        Retorna el id del articulo que causa problemas de maximo de articulos por topico, incuyendo el id 
        del topico del cual genera problemas
        */
        std::vector<int> article_topics_problem_and_topic(Topics, int);

        /*
        int select_article_diferent_day(id_article_1) = id_article_2
        Retorna el id de un articulo que se encuentre en un dia distinto al de id_article_1
        */
        int select_article_diferent_day(int);

        /*
        int select_article_diferent_dayV2(id_article, id_topic,internal_seed) = id_article
        Retorna el id de un articulo que se encuentre en un dia distinto al de id_article_1
        y que no tenga el topico id_topic
        */
        int select_article_diferent_dayV2(Topics, int, int,int);

        /*
        get_articles_from_random_sessions(internal_seed) = [[int articles session 1], [int articles session 2]]
        Retorna los id de los articulos de las sesiones 1 y sesiones 2 para realizar los movimientos de swap
        donde la sesion 1 es distinta a la sesion 2
        */
        std::vector<std::vector<int>> get_articles_from_random_sessions(int);
};

#endif
