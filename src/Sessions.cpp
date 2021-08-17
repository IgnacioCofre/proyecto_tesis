#include "../includes/class.h"

Sessions Data::create_Sessions()
{
    Sessions sessions(number_days,session_data,max_assign_per_session);
    return sessions;
}

Sessions::Sessions(int n_days, std::vector <std::vector<int>> s_data,std::vector<std::vector<std::vector<int>>> max_assign)
{
    //sesssion(number_days, number_blocks, session_data, max_assign_per_session)
    number_days = n_days;
    session_types = s_data;
    max_assign_per_session = max_assign;

    std::cout << "Sessions created" << std::endl;
}

int Sessions::max_article_session(int day, int block, int session)
{
    try {
        return max_assign_per_session[day][block][session];
        std::cout << "Out of Range error."<<std::endl;
    }
        catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
    return -1;
}

std::vector<int> Sessions::session_type(int id_type)
{
    return session_types[id_type];
}

void Sessions::show_data(){
   std::cout<< "max_assign_per_session"<<std::endl;
   int number_days = max_assign_per_session.size();
   for(int day=0; day<number_days; day++)
   {
        int number_blocks = max_assign_per_session[day].size();
        for(int block=0; block<number_blocks; block++)
        {
            int number_sessions = max_assign_per_session[day][block].size();
            for(int session=0; session<number_sessions; session++)
            {
                std::cout<<day<<","<<number_blocks<<","<<number_sessions<<std::endl;
            }
        }
   }
}
