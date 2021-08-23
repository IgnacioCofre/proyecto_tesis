#include "../includes/class.h"

Articles Data::create_Articles()
{
    Articles articles(number_articles,similarity_matrix);
    return articles;
}

Articles::Articles(int n_articles, std::vector<std::vector<int>> matrix)
{
    number_articles = n_articles;
    similarity_matrix = matrix;

    std::cout << "Articles created" << std::endl;
}

int Articles::get_number_articles()
{
    return number_articles;
}

int Articles::get_similarity(int id_article_1, int id_article_2)
{
    if((id_article_1>number_articles) || (id_article_2>number_articles))
    {
        //id_article out of range
        return -1;
    }
    return similarity_matrix[id_article_1][id_article_2];
}

std::vector<std::vector<int>> Articles::get_similarity_matrix()
{
    return similarity_matrix;
}

void Articles::show_data()
{
    std::cout <<"Show data articles" << std::endl;
    std::cout <<"Number articles: "<< number_articles << std::endl;

    std::cout <<"Similarity matrix" << std::endl;
    for(int i = 0; i<number_articles; i++)
    {
        for(int j = 0; j<number_articles; j++)
        {
            std::cout << similarity_matrix[i][j] << " ";
        }

        std::cout << std::endl; 
    }
}
