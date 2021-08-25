#ifndef SESSIONS_H
#define SESSIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>

class Sessions{
    int number_days;

    //session_types[type] = [number_max_articles, number_sessions, number_min_articles]
    std::vector <std::vector<int>> session_types;
    
    //max_assign_per_session[day][block][session] = max_number_articles
    std::vector<std::vector<std::vector<int>>> max_assign_per_session;

    //sheduling_by_room[id_session] = [Day,Block,Room,Max_articles]
    std::vector<std::vector<int>>   sheduling_by_room;

    //data_days[index] = [number_block, number_sessions]
    //std::vector <std::vector<int>> data_days;

    public:
        //sesssion(number_days, session_data, max_assign_per_session)
        Sessions(int, std::vector <std::vector<int>>, std::vector<std::vector<std::vector<int>>>);
        int max_article_session(int, int, int);
        std::vector<int> session_type(int);
        std::vector<std::vector<std::vector<int>>> get_max_article_session(void);
        std::vector<std::vector<int>> get_sheduling_by_room(void);
        void show_data(void);
};

#endif