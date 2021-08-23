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

    std::cout<<"Ants created"<<std::endl;
}
         
int Ants::get_number_ants(void)
{
    return number_ants;
}

float Ants::get_test(int id_ant, int id_article_1, int id_article_2)
{
    return ants_pheromone_matrix[id_ant][id_article_1][id_article_2];
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