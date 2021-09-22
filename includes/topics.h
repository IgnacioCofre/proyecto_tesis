#ifndef TOPICS_H
#define TOPICS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>

class Topics{
    int number_topics;
    std::vector <int> number_topics_articles;
    
    //articles_topics[id_article] = [ids topics]
    std::vector <std::vector<int>> articles_topics;

    //number_articles_by_topic[id_topic] = number of articles with the topic
    std::vector <int> number_articles_by_topic;

    public:
        Topics(int, std::vector <int>, std::vector <std::vector<int>>);
        int get_number_articles_by_topic(int);
        std::vector<int> get_article_topics(int);
        int get_number_topics(void);
        void show_data();

};

#endif