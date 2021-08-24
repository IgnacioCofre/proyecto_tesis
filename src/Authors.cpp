#include "../includes/authors.h"
#include "../includes/data.h"

Authors Data::create_Authors()
{
    Authors authors(number_articles,number_authors,number_articles_authors,articles_authors);
    return authors;
}

Authors::Authors(int n_articles, int n_authors, std::vector<int> n_articles_authors, std::vector <std::vector<int>> articles_authors)
{
    number_articles = n_articles;
    number_authors = n_authors;
    number_articles_authors = n_articles_authors;
    article_authors = articles_authors;

    std::vector <std::vector<int>> aux_author_articles(number_authors);

    for(int article=0; article<number_articles; article++)
    {   
        //std::cout << article <<std::endl;
        for(int j=0; j<number_articles_authors[article]; j++)
        {
            //std::cout << article_authors[article][j] << std::endl;
            aux_author_articles[article_authors[article][j]].push_back(article);
            
        }  
    }

    author_articles = aux_author_articles;

    //std::vector<std::vector<int>> aux_common_author(number_articles);
    std::vector<std::vector<int>> aux_common_author(number_articles, std::vector<int>(number_articles, 0));
    
    for(int author=0;author<number_authors;author++)
    {   
        int number_articles_author = author_articles[author].size();
        if(number_articles_author > 1)
        {
            for(int i=0; i<number_articles_author; i++)
            {
                for(int j=i+1; j<number_articles_author; j++)
                {
                    aux_common_author[author_articles[author][i]][author_articles[author][j]] += 1;
                    aux_common_author[author_articles[author][j]][author_articles[author][i]] += 1;
                }    
            }
        }
    }

    common_author = aux_common_author;

    std::cout << "Authors created" << std::endl;

}

int Authors::get_number_common_author(int id_article_1, int id_article_2)
{
    return common_author[id_article_1][id_article_2];

    /*
    if(id_article_1 != id_article_2)
    {
        return common_author[id_article_1][id_article_2];
    }
    
    std::cerr << "reading same id_article in diferent sessions" << std::endl;
    std::cerr << "Id_article: "<< id_article_1 << std::endl;
    exit(EXIT_FAILURE);
    */
}

void Authors::show_data()
{
    std::cout<< "Show data authors" << std::endl;
    std::cout<< "Number of articles: "<<number_articles << std::endl;
    std::cout<< "Number of authors:  "<<number_authors << std::endl;
    
    std::cout << "id_articles per author:" << std::endl;
    for(int author=0; author<number_authors; author++)
    {   
        //std::cout << author << std::endl;
        //print id_articles de un autor
        int number_author_articles = author_articles[author].size();
        //std::cout << number_author_articles << std::endl;
        for(int article=0; article<number_author_articles; article++)
        {
            std::cout<< author_articles[author][article] << " ";
        } 
        std::cout << std::endl;
    }

    std::cout << "Matriz of number of common authors:" << std::endl;
    for(int i=0; i<number_articles; i++)
    {
        for(int j=0; j<number_articles; j++)
        {
            std::cout << common_author[i][j] << " ";
        }
        std::cout<< std::endl;
    }
}

std::vector<int> Authors::get_author_articles(int id_author)
{
    if(id_author < number_authors)
    {
        return author_articles[id_author];
    }
    
    std::cerr << "id_author out off range" << std::endl;
    exit(EXIT_FAILURE);
}

int Authors::get_number_authors()
{
    return number_authors;
}