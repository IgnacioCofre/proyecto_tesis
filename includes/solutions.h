#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>

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
        int get_number_days(void);
        std::vector<int> get_article_asignation(int);
        void show_solution(void);
};

#endif