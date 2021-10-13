#ifndef TOPICS_H
#define TOPICS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <math.h>

class Topics{
    int number_topics, number_days;
    std::vector <int> number_topics_articles;
    
    //articles_topics[id_article] = [ids topics]
    std::vector <std::vector<int>> articles_topics;

    //number_articles_by_topic[id_topic] = number of articles with the topic
    std::vector <int> number_articles_by_topic;

    //article_by_topic[id_topic] = [ids articles with topic]
    std::vector<std::vector<int>> article_by_topic;

    //max_article_topic[id_topic] = int number of max article that can be assing 
    //in a day by topic 
    std::vector<int> max_article_topic;

    public:
        Topics(int, std::vector <int>, std::vector <std::vector<int>>, int);
        int get_number_articles_by_topic(int);
        std::vector<int> get_article_topics(int);
        int get_number_topics(void);
        void show_data();
        
        /*
        get_max_per_day(id_topic);
        Retorna el maximo de articulos que se pueden asignar por dia del topico id_topic
        */
        int get_max_per_day(int);

        /*
        std::vector<int> get_articles_by_topic(id_topico) = [ids articles]
        Retorna los id de los articulos que contengan el topico id_topico 
        */
        std::vector<int> get_articles_by_topic(int);

};

#endif