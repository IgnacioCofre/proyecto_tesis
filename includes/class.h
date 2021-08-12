#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include "function.h"

class Data {
    int number_articles;
    int number_type_sessions;
    int number_days;
    int number_topics;
    int number_authors;
    
    std::vector <std::vector<int>> similarity_matrix;
    std::vector <std::vector<int>> session_data;
    std::vector <std::vector<int>> data_days;
    
    std::vector <std::vector<std::vector<int>>> data_days_timeblock;
    std::vector <std::vector<int>> data_timeblock;

    std::vector <int> number_topics_articles;
    std::vector <std::vector<int>> articles_topics;

    std::vector <int> number_articles_authors;
    std::vector <std::vector<int>> articles_authors; 

    public:
        void read_input_file(const char *);
        void show_data(void); 
};

class Articles{
    int id, number_topics, number_authors;
    std::vector<int> article_authors;
    std::vector<int> article_topics;
};

class Sessions{
    int id, number_session, max_article, min_article;
};

class Topics{
    int id;
    std::vector<int> topic;
};

class Authors{
    int id;
    std::vector<int> articles;
};

class Timetabling{
    int time_blocks, rooms, days;
    std::vector<std::vector<std::vector<int>>> timetabling;
};