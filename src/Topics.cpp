#include "../includes/class.h"

Topics Data::create_Topics()
{
    Topics topics(number_topics, number_topics_articles, articles_topics);
    return topics;
}

Topics::Topics(int n_topics, std::vector <int> n_topics_articles, std::vector <std::vector<int>> topics)
{
    number_topics = n_topics;
    number_topics_articles = n_topics_articles;
    articles_topics = topics;

    std::vector<int> aux_number_articles_by_topic(number_topics,0);

    int number_articles = articles_topics.size();
    for(int id_article=0; id_article<number_articles; id_article++)
    {
        int n_topics = articles_topics[id_article].size();
        for(int topic=0; topic<n_topics; topic++)
        {
            aux_number_articles_by_topic[articles_topics[id_article][topic]] +=1;
        }
    }

    number_articles_by_topic = aux_number_articles_by_topic;
    std::cout << "Topics created" << std::endl;
}

int Topics::get_number_articles_by_topic(int id_topic)
{
    if(id_topic>number_topics)
    {
        std::cout<<"Problem with id topic: "<<id_topic<<std::endl;
        return -1;
    }

    return number_articles_by_topic[id_topic];
}

void Topics::show_data()
{
    std::cout<< "Show data topics" << std::endl;
    std::cout<< "Topics by article" << std::endl;
    
    int number_articles = articles_topics.size();
    for(int id_article=0; id_article<number_articles; id_article++)
    {
        int n_topics = articles_topics[id_article].size();
        for(int topic=0; topic<n_topics; topic++)
        {
            number_articles_by_topic[articles_topics[id_article][topic]] +=1;
        }
    }

    std::cout<< "Number articles by topic [Id_topic,number_articles]" << std::endl;
    for(int topic=0; topic<number_topics; topic++)
    {
        std::cout<<topic<<","<<number_articles_by_topic[topic]<< std::endl;
    }
}