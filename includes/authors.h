#ifndef AUTHORS_H
#define AUTHORS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>

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

        /*
        get_author_articles(id_author) = [id_articles of authors]
        Retorna un vector de enteros que corresponden a los id de los articulos del autor id_author
        a partir de la lista author_articles
        */  
        std::vector<int> get_author_articles(int); 
        int get_number_authors(void);
        
        /*
        get_atricle_autors(id_article)
        Retorna un vector de enteros con los id de los autores del articulo id_article
        a partir de la lista de vectores article_authors
        */
        std::vector<int> get_article_authors(int);

};

#endif