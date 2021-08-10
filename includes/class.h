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
    
    std::vector <std::vector<int>> similarity_matrix;
    std::vector <std::vector<int>> session_data;
    std::vector <std::vector<int>> data_days;
    
    std::vector <std::vector<std::vector<int>>> data_days_timeblock;
    std::vector <std::vector<int>> data_timeblock;

    std::vector <int> number_topics_articles;
    std::vector <std::vector<int>> articles_topics;

    public:
        void read_input_file(const char *);
        void show_data(void); 
};

