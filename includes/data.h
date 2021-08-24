#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>

#include "authors.h"
#include "sessions.h"
#include "articles.h"
#include "topics.h"

class Data {
    int number_articles;
    int number_type_sessions;
    int number_days;
    int number_topics;
    int number_authors;
    
    //similarity_matrix[id_article1][id_article2] = number
    std::vector <std::vector<int>> similarity_matrix;

    //session_data[type] = [number_max_articles, number_sessions, number_min_articles]
    std::vector <std::vector<int>> session_data;

    //data_days[index] = [number_block, number_sessions]
    std::vector <std::vector<int>> data_days;
    
    std::vector <std::vector<std::vector<int>>> data_days_timeblock;
    std::vector <std::vector<int>> data_timeblock;

    //number_topics_articles[id_article] = number topics
    std::vector <int> number_topics_articles;

    //articles_topics[id_article] = [ids_topics]
    std::vector <std::vector<int>> articles_topics;

    //max_assign_per_session[day,block,session] = max_number_articles
    std::vector<std::vector<std::vector<int>>> max_assign_per_session;

    //number_articles_authors[id_article] = int number of authors
    std::vector <int> number_articles_authors;
    //articles_authors[id_authors] = [ids_authors of paper]
    std::vector <std::vector<int>> articles_authors;

    public:
        void read_input_file(const char *);
        void show_data(void); 
        int get_similarity(int, int);
        int get_number_days(void);
        int get_number_articles(void);

        Authors create_Authors(void);
        Sessions create_Sessions(void);
        Articles create_Articles(void);
        Topics create_Topics(void);
};

#endif