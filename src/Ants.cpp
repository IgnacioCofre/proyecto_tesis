#include "../includes/ants.h"

//Ants(number_ants,max_pheromone,min_pheromone,number_articles)
Ants::Ants(int n_ants,float mx_pheromone,float mn_pheromone,int n_articles, float alp, float b, float lk_c,std::vector<std::vector<int>> s_matrix)
{
    number_ants = n_ants;
    max_pheromone = mx_pheromone;
    min_pheromone = mn_pheromone;
    alpha = alp;
    beta = b;
    number_articles = n_articles;
    Lk_constant = lk_c;
    similarity_matrix = s_matrix;

    //std::vector<std::vector<std::vector<float>>> aux_ant_matrix(number_ants,std::vector<std::vector<float>>(n_articles,std::vector<float>(n_articles,mn_pheromone)));
    //ants_pheromone_matrix = aux_ant_matrix;

    std::vector<std::vector<float>> aux_matrix(n_articles,std::vector<float>(n_articles,mn_pheromone));
    pheromone_matrix = aux_matrix;

    std::vector<std::vector<std::vector<float>>> aux_ant_matrix(number_ants,aux_matrix);
    ants_pheromone_matrix = aux_ant_matrix;

    std::vector<std::vector<int>> aux_ant_solution(number_ants); 
    solution_ant = aux_ant_solution;

    std::cout<<"Ants created: "<<number_ants<<std::endl;
}
         
int Ants::get_number_ants(void)
{
    return number_ants;
}

float Ants::get_test(int id_ant, int id_article_1, int id_article_2)
{
    return ants_pheromone_matrix[id_ant][id_article_1][id_article_2];
}

int Ants::save_solution(int id_ant,int id_article)
{
    if((id_ant>number_ants)||(id_article>number_articles))
    {
        std::cout <<"Ivalid [Id Ant,Id Article]: ["<<id_ant<<","<<id_article<<"]"<<std::endl;
        return 0;
    }
    
    solution_ant[id_ant].push_back(id_article);
    return 1;
}

std::vector<int> Ants::get_solution_ant(int id_ant)
{
    if(id_ant>number_ants)
    {
        std::cout <<"Ivalid Id Ant: "<<id_ant<<std::endl;
        return {0};
    }
    return solution_ant[id_ant];

}

int Ants::get_next_article(int id_article_1, std::vector<int> list_j)
{   
    int number_j = list_j.size();
    std::vector<float> p_ij(number_j,0);
    float sum_p_ij = 0;
    
    for(int j=0; j<number_j; j++)
    {
        int id_article_2 = list_j[j];
        float tau_ij = pheromone_matrix[id_article_1][id_article_2];
        float eta_ij = similarity_matrix[id_article_1][id_article_2];

        float numerador = pow(tau_ij,alpha)*pow(eta_ij,beta);
        p_ij[j] = numerador;
        sum_p_ij += numerador;
    }

    /* 
    en caso de que todos los arcos de un articulo i a todos los articulos j tengan ponderacion 0
    se elije el siguiente articulo en la solucion de forma aleatoria
    */
    if(sum_p_ij == 0)
    {
        int next_j = (int) std::rand() % number_j;
        //std::cout <<"Especial case: "<< next_j <<std::endl;
        return next_j; 
    }

    float new_random = (float) rand()/RAND_MAX;

    float acumulated = (p_ij[0]/sum_p_ij);
    int next_i = 0;
    
    while(new_random - acumulated > 0)
    {
        next_i++;
        acumulated += (p_ij[next_i]/sum_p_ij);
        
    }
    
    return next_i;
}

int Ants::solution_quality(int id_ant, std::vector<std::vector<std::vector<int>>> max_assign_per_session)
{
    std::vector<int> solution = solution_ant[id_ant];
    int iter_solution = 0;
    int total_quality = 0;
    std::vector<std::vector<int>> benefit_per_session;
    int number_days = max_assign_per_session.size();

    for(int day=0; day<number_days; day++)
    {
        int number_blocks = max_assign_per_session[day].size();
        for(int block=0; block<number_blocks; block++)
        {
            int number_sessions = max_assign_per_session[day][block].size();
            for(int session=0; session<number_sessions; session++)
            {
                int benefit_session = 0;
                int number_articles = max_assign_per_session[day][block][session];

                for(int iter_1=iter_solution; iter_1<number_articles + iter_solution; iter_1++)
                {
                    for(int iter_2=iter_1+1; iter_2<number_articles + iter_solution; iter_2++)
                    {
                        int id_article_1 = solution[iter_1];
                        int id_article_2 = solution[iter_2];

                        int benefit_articles = similarity_matrix[id_article_1][id_article_2];
                
                        total_quality += benefit_articles;
                        benefit_session += benefit_articles;           
                    }   
                }

                iter_solution += number_articles;
                benefit_per_session.push_back({day,block,session,benefit_session});
            }
        }
    }

    return total_quality;
}
