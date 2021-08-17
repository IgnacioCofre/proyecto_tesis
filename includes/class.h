#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>

class Authors{
    public:
        int number_articles;
        int number_authors;
        //number_articles_authors[id_article] = int number of authors
        std::vector <int> number_articles_authors;
        //articles_authors[id_article] = [ids_authors of paper]
        std::vector <std::vector<int>> article_authors;
        //authors_articles[id_author] = [ids_articles of the author]
        std::vector <std::vector<int>> author_articles;
        //common_author(id_article_1,id_article_2) = {0: no common author; 1: common author}    
        std::vector<std::vector<int>> common_author; 

        Authors(int, int, std::vector<int>, std::vector <std::vector<int>>);
        int get_number_common_author(int, int);
        void show_data(void);   
        std::vector<int> get_author_articles(int); 
        int get_number_authors(void);

};

class Sessions{
    int number_days;

    //session_types[type] = [number_max_articles, number_sessions, number_min_articles]
    std::vector <std::vector<int>> session_types;
    
    //max_assign_per_session[day,block,session] = max_number_articles
    std::vector<std::vector<std::vector<int>>> max_assign_per_session;

    //data_days[index] = [number_block, number_sessions]
    //std::vector <std::vector<int>> data_days;

    public:
        //sesssion(number_days, session_data, max_assign_per_session)
        Sessions(int, std::vector <std::vector<int>>, std::vector<std::vector<std::vector<int>>>);
        int max_article_session(int, int, int);
        std::vector<int> session_type(int);
        void show_data(void);
};

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

    std::vector <int> number_topics_articles;
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
};

class Solutions{
    int number_days;
    int number_articles;

    //data_day[day] = [number_block, number_sessions]
    std::vector<std::vector<int>> data_day;
    //scheduling[Day][Block][Session] = [ids articles]
    std::vector<std::vector<std::vector<std::vector<int>>>> scheduling;
    //article_asignation[id_aticle] = [day,block,session] of asignation 
    std::vector<std::vector<int>> article_asignation; 

    public:
        void read_solution(const char *); 
        std::vector<std::vector<std::vector<std::vector<int>>>> get_scheduling(void);
        int get_number_articles(void);
        std::vector<int> get_article_asignation(int);
        void show_solution(void);
};

class Articles{
    int id, number_topics, number_authors;
    std::vector<int> article_authors;
    std::vector<int> article_topics;
};

class Topics{
    int id;
    std::vector<int> topic;
};

class Timetabling{
    int time_blocks, rooms, days;
    std::vector<std::vector<std::vector<int>>> timetabling;
};

class Validator
{
    std::vector<std::string>  comments;

    public:
        //Verifica que no hayan articulos de un mismo autor en sesiones paralelas
        void articles_in_diferent_sessions(Data, Authors, Solutions);
        
        //Verifica que todos los articulos esten asignados solo una vez
        void article_assignment(Data, Solutions);

        //Verifica que se cumpla la capacidad de articulos por sesion
        void capacity_session(Sessions,Solutions);

        std::vector<std::string> get_comments(void);
        void show_comments(void);    
};