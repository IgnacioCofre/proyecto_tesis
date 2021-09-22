#ifndef ARTICLES_H
#define ARTICLES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>

class Articles{
    int number_articles;
    std::vector<std::vector<int>> similarity_matrix;

    public:
        Articles(int, std::vector<std::vector<int>>);
        int get_number_articles(void);
        int get_similarity(int,int);
        std::vector<std::vector<int>> get_similarity_matrix();
        void show_data(void);
};


#endif