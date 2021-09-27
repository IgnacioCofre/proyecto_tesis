#include "../includes/data.h"
#include "../includes/topics.h"

Topics Data::create_Topics()
{
    Topics topics(number_topics, number_topics_articles, articles_topics,number_days);
    return topics;
}

Topics::Topics(int n_topics, std::vector <int> n_topics_articles, std::vector <std::vector<int>> topics, int n_days)
{
    number_topics = n_topics;
    number_topics_articles = n_topics_articles;
    articles_topics = topics;
    number_days = n_days;

    std::vector<int> aux_number_articles_by_topic(number_topics,0);

    int number_articles = articles_topics.size();
    for(int id_article=0; id_article<number_articles; id_article++)
    {
        int n_topics = articles_topics[id_article].size();
        for(int topic=0; topic<n_topics; topic++)
        {
            int id_topic = articles_topics[id_article][topic];
            aux_number_articles_by_topic[id_topic] +=1;
            //std::cout<<id_topic<<",";
        }
        //std::cout<<std::endl;
    }

    number_articles_by_topic = aux_number_articles_by_topic;

    std::vector<int> aux_max_article_per_day(number_topics,0);
    for(int id_topic=0; id_topic<number_topics; id_topic++)
    {
        int article_by_topic = aux_number_articles_by_topic[id_topic];
        if(number_days == 1)
        {
            aux_max_article_per_day[id_topic] = article_by_topic;
        }
        else
        {
            if(article_by_topic>20)
            {
                aux_max_article_per_day[id_topic] = ceil(article_by_topic/2.0);    
            }
            else
            {
                aux_max_article_per_day[id_topic] = article_by_topic;
            }
        }
    }

    max_article_topic = aux_max_article_per_day;

    std::cout << "Topics created" << std::endl;
}

int Topics::get_number_articles_by_topic(int id_topic)
{
    if(id_topic>=number_topics)
    {
        std::cout<<"Problem with id topic: "<<id_topic<<std::endl;
        return -1;
    }

    return number_articles_by_topic[id_topic];
}

int Topics::get_number_topics()
{
    return number_topics;
}

std::vector<int> Topics::get_article_topics(int id_article)
 {
    int number_articles = articles_topics.size();
    if(id_article<number_articles)
    {
        return articles_topics[id_article];
    }
    else
    {
        std::cout << "id article not found: " << id_article << std::endl;
        std::vector<int> aux = {0};
        return aux;
    }
    
 }

void Topics::show_data()
{
    std::cout<< "Show data topics" << std::endl;
    std::cout<< "Topics by article" << std::endl;

    std::cout<< "Number articles by topic [Id_topic,number_articles]" << std::endl;
    for(int topic=0; topic<number_topics; topic++)
    {
        std::cout<<topic<<","<<number_articles_by_topic[topic]<< std::endl;
    }
}

int Topics::get_max_per_day(int id_topic)
{
    if(id_topic>=number_topics)
    {
        std::cout<<"Problem with id topic: "<<id_topic<<std::endl;
        return -1;
    }

    return max_article_topic[id_topic];
}