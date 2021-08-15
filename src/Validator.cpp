#include "../includes/class.h"

void Validator::solution_validation(Data data, Authors authors, Solutions solution)
{
    std::cout << "Validating authors and articles on diferent sessions" << std::endl;
    std::vector<std::vector<std::vector<std::vector<int>>>> scheduling = solution.get_scheduling();
    
    
    int number_days =  data.get_number_days();
    int number_articles = data.get_number_articles();
    int number_authors = authors.get_number_authors();
    //std::cout << authors.get_number_common_author(19,1) << std::endl;
    
    //Se verifica que no hayan articulos de un mismo autor en sesiones paralelas
    int comparison_method = 1; 
    
    /* Esto es muy costoso de hacer pero funciona*/
    if(comparison_method == 0)
    {
        for(int day=0; day<number_days; day++)
        {
            int number_blocks = scheduling[day].size();
            for(int block=0; block<number_blocks; block++)
            {
                int number_sessions = scheduling[day][block].size();
                if(number_sessions>1)
                {
                    for(int session_1=0; session_1<number_sessions; session_1++)
                    {
                        for(int session_2=session_1+1; session_2<number_sessions; session_2++)
                        {
                            int n_articles_session_1 = scheduling[day][block][session_1].size();
                            int n_articles_session_2 = scheduling[day][block][session_2].size();

                            for(int id_article_1 =0; id_article_1<n_articles_session_1; id_article_1++)
                            {
                                for(int id_article_2 =0; id_article_2<n_articles_session_2; id_article_2++)
                                {
                                    int id1 = scheduling[day][block][session_1][id_article_1];
                                    int id2 = scheduling[day][block][session_2][id_article_2];
                                    if(authors.get_number_common_author(id1,id2)>0)
                                    {
                                        std::cout << "Problem articles: " << id1 << " " << id2 << std::endl;
                                        comments.push_back("Problem articles: "+std::to_string(id1)+" "+std::to_string(id2));  
                                    }
                                }
                            }

                        }
                    }       
                }
            }
        }
    }
    
    /* Esto es un poco mas optimo*/
    else
    {
        if(solution.get_number_articles() != number_articles)
        {
            comments.push_back("Error: not all articles are assing to a session");
        }
        else
        {
            for(int author= 0; author< number_authors; author++)
            {
                std::vector<int> list_articles = authors.get_author_articles(author);
                int n_articles = list_articles.size();

                //std::cout<<"Author: "<<author<<std::endl;
                //std::cout<<list_articles[0]<<std::endl;

                if(n_articles>1)
                {
                    //std::cout<<"Author con mas de un articulo: "<<author<<std::endl;
                    //std::cout<<n_articles<<std::endl;
                    
                    for(int i=0; i<n_articles; i++)
                    {   
                        int id_article_1 = list_articles[i];
                        //std::cout<<id_article_1<<std::endl;
                        std::vector<int> article1_assing = solution.get_article_asignation(id_article_1);

                        for(int j=i+1; j<n_articles; j++)
                        {
                            int id_article_2 = list_articles[j];
                            std::vector<int> article2_assing = solution.get_article_asignation(id_article_2);

                            //se verifica primero que los dias y el bloque de dos articulos de un mismo autor sean iguales
                            if((article1_assing[0] == article2_assing[0]) & (article1_assing[1] == article2_assing[1]))
                            {   
                                //si las sesiones son diferentes no se cumplen con las restricciones
                                if(article1_assing[2] != article2_assing[2])
                                {
                                    //std::cout << "Problem articles: " << id_article_1 << " " << id_article_2 << std::endl;
                                    comments.push_back("Problem articles: "+std::to_string(id_article_1)+" "+std::to_string(id_article_2)); 
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<std::string> Validator::get_comments()
{
    return comments;
}

void Validator::show_comments()
{
    std::cout <<"Coments validator:" <<std::endl;
    int n_lines = comments.size();

    for(int line=0; line<n_lines; line++)
    {
        std::cout<< comments[line] <<std::endl;
    }
}