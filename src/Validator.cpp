#include "../includes/class.h"

void Validator::solution_validation(Data data, Authors authors, Solutions solution)
{
    std::cout << "Validating solution" << std::endl;
    
    std::vector<std::vector<std::vector<std::vector<int>>>> scheduling = solution.get_scheduling();
    
    std::cout << "Validating authors" << std::endl;
    int number_days =  data.get_number_days();

    //std::cout << authors.get_number_common_author(19,1) << std::endl;
    
    /* Esto es muy costoso de hacer pero funciona*/
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

std::vector<std::string> Validator::get_comments()
{
    return comments;
}