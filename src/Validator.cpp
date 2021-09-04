#include "../includes/validator.h"

int Validator::articles_in_diferent_sessions(Data data, Authors authors, std::vector<std::vector<std::vector<std::vector<int>>>> scheduling)
{
    //std::cout << "Validating authors and articles on diferent sessions" << std::endl;
    //comments.push_back("Validating authors and articles on diferent sessions");
    //std::vector<std::vector<std::vector<std::vector<int>>>> scheduling = solution.get_scheduling();

    bool show_conflict = false;
    
    int number_days =  data.get_number_days();
    int number_articles = data.get_number_articles();
    int number_authors = authors.get_number_authors();
    std::vector<std::vector<int>> article_asignation(number_articles,{-1,-1,-1});

    int number_problems = 0;
    
    //Se verifica que no hayan articulos de un mismo autor en sesiones paralelas
    //int comparison_method = 1; 
    
    for(int day=0; day<number_days; day++)
    {
        int number_blocks = scheduling[day].size();
        for(int block=0; block<number_blocks; block++)
        {
            int number_sessions = scheduling[day][block].size();
            for(int session=0; session<number_sessions; session++)
            {
                int n_articles_session_1 = scheduling[day][block][session].size();
                for(int iter_article =0; iter_article<n_articles_session_1; iter_article++)
                {
                    int id_article = scheduling[day][block][session][iter_article];
                    article_asignation[id_article] = {day,block,session};
                }
            }       
        }
    }

    /*
    Se revisan que los articulos de cada autor no se encuentren en sesiones 
    paralelas
    */
    for(int author= 0; author< number_authors; author++)
    {
        std::vector<int> list_articles = authors.get_author_articles(author);
        int n_articles = list_articles.size();

        if(n_articles>1)
        {
            for(int i=0; i<n_articles; i++)
            {   
                int id_article_1 = list_articles[i];
                int day_article_1 = article_asignation[id_article_1][0];
                int block_article_1 = article_asignation[id_article_1][1];
                int sesison_article_1 = article_asignation[id_article_1][2];    

                for(int j=i+1; j<n_articles; j++)
                {
                    int id_article_2 = list_articles[j];
                    int day_article_2 = article_asignation[id_article_2][0];
                    int block_article_2 = article_asignation[id_article_2][1];
                    int sesison_article_2 = article_asignation[id_article_2][2];   

                    //se verifica primero que los dias y el bloque de dos articulos de un mismo autor sean iguales
                    if((day_article_1 == day_article_2) & (block_article_1 == block_article_2))
                    {   
                        //si las sesiones son diferentes no se cumplen con las restricciones
                        if(sesison_article_1 != sesison_article_2)
                        {
                            //std::cout << "Problem articles: " << id_article_1 << " " << id_article_2 << std::endl;
                            number_problems += 1;
                            if(show_conflict)
                            {
                                //comments.push_back("Problem articles: "+std::to_string(id_article_1)+" "+std::to_string(id_article_2)); 
                                std::cout<<"Problem articles: "<<id_article_1<<" "<<id_article_2<<std::endl;
                            }
                        }
                    }
                }
            }
        }

        std::vector<int>().swap(list_articles);
    }

    std::vector<std::vector<int>>().swap(article_asignation);
    return number_problems;
    /* Esto es muy costoso de hacer pero funciona*/
    /*
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
    */
}

void Validator::article_assignment(Data data, Solutions solution)
{
    std::cout << "Validating articles are only assing to one session" << std::endl;
    comments.push_back("Validating articles are only assing to one session");

    int number_articles = data.get_number_articles();
    for(int id_article = 0; id_article<number_articles; id_article++)
    {
        std::vector<int> article_assing = solution.get_article_asignation(id_article);
        if(article_assing[0] == -1)
        {
            //std::cout<< "Article is not assing: "<< id_article<< std::endl;
            comments.push_back("Article is not assing: "+std::to_string(id_article));
        } 
    }
}

void Validator::capacity_session(Sessions sessions,std::vector<std::vector<std::vector<std::vector<int>>>> scheduling)
{
    std::cout << "Validating articles capacity of session" << std::endl;
    comments.push_back("Validating articles capacity of session");

    //std::vector<std::vector<std::vector<std::vector<int>>>> scheduling = solution.get_scheduling();
    
    int number_days = scheduling.size();
    //std::cout<<number_days<<std::endl;
    for(int day=0; day<number_days; day++)
    {
        int number_blocks = scheduling[day].size();
        //std::cout<<number_blocks<<std::endl;
        for(int block=0; block<number_blocks; block++)
        {
            int number_sessions =  scheduling[day][block].size();
            //std::cout<<number_sessions<<std::endl;

            for(int session=0; session<number_sessions; session++)
            {   
                int max_size_session = sessions.max_article_session(day,block,session);
                //std::cout<<max_size_session<<std::endl;
                if(max_size_session != -1)
                {   
                    int size_session = scheduling[day][block][session].size();
                    if(size_session>max_size_session)
                    {
                        //std::cout<< "Session out of max articles: "+std::to_string(day)+","+std::to_string(block)+","+std::to_string(session)<< std::endl;
                        comments.push_back("Session out of max articles: "
                        +std::to_string(day)+","+std::to_string(block)+","+std::to_string(session));
                    }
                }
                else
                {
                    //std::cout<< "Session can't be assined: "+std::to_string(day)+","+std::to_string(block)+","+std::to_string(session)<< std::endl;
                    comments.push_back("Session can be assined: "
                    +std::to_string(day)+","+std::to_string(block)+","+std::to_string(session));
                }
            }
        }
    }
}

int Validator::capacity_topics(Topics topics,std::vector<std::vector<std::vector<std::vector<int>>>> scheduling)
{
    
    bool show_problems = false;
    int number_days = scheduling.size();
    int number_problems = 0;
    int number_articles_over_max_capacity = 0;

    //std::cout << "Validating capacity of topics per day" << std::endl;
    //comments.push_back("Validating capacity of topics per day");
    if(show_problems)
    {
        std::cout<<"Day out of max topics [day,id_topic,number_articles]"<<std::endl;
    }

    if(number_days > 1)
    {
        int number_topics = topics.get_number_topics();
        //std::vector<std::vector<std::vector<std::vector<int>>>> scheduling = solutions.get_scheduling();

        //counter_topics_per_day[day][id_topic] = number of articles with topic id_topic
        std::vector<std::vector<int>> counter_topics_per_day(number_days,std::vector<int>(number_topics));

        for(int day=0; day<number_days; day++)
        {
            int number_blocks = scheduling[day].size();
            for(int block=0; block<number_blocks; block++)
            {
                int number_sessions = scheduling[day][block].size();
                for(int session=0; session<number_sessions; session++)
                {
                    int number_articles = scheduling[day][block][session].size();
                    for(int article=0; article<number_articles; article++)
                    {
                        int id_article = scheduling[day][block][session][article];
                        std::vector<int> topics_in_article = topics.get_article_topics(id_article);
                        int number_topics_in_article = topics_in_article.size();
                        for(int topic=0; topic<number_topics_in_article; topic++)
                        {
                            int id_topic = topics_in_article[topic];
                            counter_topics_per_day[day][id_topic] += 1;
                        }
                        
                    }
                }  
            }
        }
        /* Mostrar cantidad de aticulos por dia que incluyan cierto topico
        for(int day=0; day<number_days; day++)
        {
            for(int topic=0; topic<number_topics; topic++)
            {
                std::cout << day <<","<<topic<<","<<counter_topics_per_day[day][topic]<<std::endl;
            }
        }
        */
        for(int id_topic=0; id_topic<number_topics; id_topic++)
        {
            int number_articles_with_topic = topics.get_number_articles_by_topic(id_topic);
            if(number_articles_with_topic>20)
            {
                for(int day=0; day<number_days; day++)
                {   
                    //Caso en que no se cumpla el limite de articulos de cierto topico
                    int max_topic_per_day = ceil(number_articles_with_topic/2.0);
                    if(counter_topics_per_day[day][id_topic]>max_topic_per_day)
                    {
                        /*
                        se suman la cantidad de articulos que
                        se pasan del maximo permitido por dia
                        */
                        number_articles_over_max_capacity += (counter_topics_per_day[day][id_topic]-max_topic_per_day);
                        number_problems +=1;
                        if(show_problems)
                        {
                            std::cout<<"["<<day<<"," <<id_topic<<","<<counter_topics_per_day[day][id_topic]<<"]"<<std::endl;
                        }

                        /*
                        std::cout << day <<","<<id_topic<<","<<counter_topics_per_day[day][id_topic]<<std::endl;
                        comments.push_back("Day out of max topics [day,id_topic,number_articles]: ["
                        +std::to_string(day)+","
                        +std::to_string(id_topic)+","
                        +std::to_string(counter_topics_per_day[day][id_topic]));
                        */
                    }
                    
                }
            }
        }
    }

    return number_articles_over_max_capacity;   
}

int Validator::solution_benefit(Articles articles,std::vector<std::vector<std::vector<std::vector<int>>>> scheduling)
{
    //std::cout << "Calculating the benefit of the solution" << std::endl;
    
    bool show_coments = false;
    int total_quality = 0;
    std::vector<std::vector<int>> benefit_per_session;
    int number_days = scheduling.size();

    for(int day=0; day<number_days; day++)
    {
        int number_blocks = scheduling[day].size();
        for(int block=0; block<number_blocks; block++)
        {
            int number_sessions = scheduling[day][block].size();
            for(int session=0; session<number_sessions; session++)
            {
                int benefit_session = 0;
                int number_articles = scheduling[day][block][session].size();
                for(int article_1=0; article_1<number_articles; article_1++)
                {
                    for(int article_2=article_1+1; article_2<number_articles; article_2++)
                    {
                        int id_article_1 = scheduling[day][block][session][article_1];
                        int id_article_2 = scheduling[day][block][session][article_2];

                        int benefit_articles = articles.get_similarity(id_article_1,id_article_2);
                        if(benefit_articles == -1)
                        {
                            comments.push_back("Error id articles: "
                            +std::to_string(id_article_1)+","
                            +std::to_string(id_article_2));
                            std::cout<<"Error"<<std::endl;    
                        }

                        total_quality += benefit_articles;
                        benefit_session += benefit_articles;
            
                    }   

                    
                }

                benefit_per_session.push_back({day,block,session,benefit_session});
            }
        }
    }

    if(show_coments)
    {
        int number_total_sessions = benefit_per_session.size();
        std::cout<<"Benefit per session [Day,Block,Session]:benefit"<<std::endl;
        for(int i=0;i<number_total_sessions;i++)
        {
            std::cout<<
            "["<<
            benefit_per_session[i][0]<<","<<
            benefit_per_session[i][1]<<","<<
            benefit_per_session[i][2]<<
            "]:"<<
            benefit_per_session[i][3]<<std::endl;
        }

        std::cout<<"Total benefit solution: "<<total_quality<<std::endl;
    }

    //benefit_per_session.clear();
    std::vector<std::vector<int>>().swap(benefit_per_session);
    return total_quality;
}

std::vector<std::string> Validator::get_comments()
{
    return comments;
}

void Validator::show_comments()
{
    std::cout <<"Comments validator:" <<std::endl;
    
    int n_lines = comments.size();
    if(n_lines>0)
    {
        for(int line=0; line<n_lines; line++)
        {
            std::cout<< comments[line] <<std::endl;
        }   
    }
    else
    {
        std::cout <<"No comments" <<std::endl;
    }
    
}
    
    