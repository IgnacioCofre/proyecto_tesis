#include "../includes/ants.h"

//Ants(number_ants,max_pheromone,min_pheromone,number_articles, alpha, beta,articles.get_similarity_matrix(),vapor_factor, c_factor,gamma_fator,epsilon_fator,e_max, seed)
Ants::Ants(int n_ants,float mx_pheromone,float mn_pheromone,int n_articles, float alp, float b,std::vector<std::vector<int>> s_matrix, float vapor, float c,int e_max, unsigned int seed_param)
{
    number_ants = n_ants;
    max_pheromone = mx_pheromone;
    min_pheromone = mn_pheromone;
    alpha = alp;
    beta = b;
    number_articles = n_articles;
    c_factor = c;
    similarity_matrix = s_matrix;
    vapor_factor = vapor;
    seed = seed_param;
    //gamma_factor = gamma;
    //epsilon_factor = epsilon;
    e = e_max;

    /*Matriz de  feromonas se inicializa con el valor maximo de la feromona*/
    std::vector<std::vector<float>> aux_matrix(n_articles,std::vector<float>(n_articles,max_pheromone));
    pheromone_matrix = aux_matrix;

    //std::vector<std::vector<int>> aux_ant_solution(number_ants); 
    //solution_ant = aux_ant_solution;

    //std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> aux_ant_solution_scheduling(number_ants);
    //ant_solution_scheduling = aux_ant_solution_scheduling;

    std::cout<<"Ants created: "<<number_ants<<std::endl;
}
         
int Ants::get_number_ants(void)
{
    return number_ants;
}

float Ants::get_test(int id_article_1, int id_article_2)
{
    return pheromone_matrix[id_article_1][id_article_2];
}

int Ants::get_next_article(int id_article_1, std::vector<int> list_j)
{   
    int size_list_j = list_j.size();
    std::vector<float> p_ij(size_list_j,0);
    float sum_p_ij = 0;
    
    for(int j=0; j<size_list_j; j++)
    {
        int id_article_2 = list_j[j];
        float tau_ij = pheromone_matrix[id_article_1][id_article_2];
        
        /*
        Cambio en la ponderacion del valor heuristico para la probabilidad de transicion
        para evitar valorres de eta_ij = 0
        eta_ij = b_ij*10+1
        */
        int b_ij = similarity_matrix[id_article_1][id_article_2];
        float eta_ij = b_ij*10.0 + 1.0;
        
        float numerador = pow(tau_ij,alpha)*pow(eta_ij,beta);
        p_ij[j] = numerador;
        sum_p_ij += numerador;
    }

    srand(seed);
    float new_random = (float) rand()/RAND_MAX;

    float acumulated = (p_ij[0]/sum_p_ij);
    int next_i = 0;
    
    while((new_random > acumulated) && (next_i<size_list_j-1))
    {
        next_i++;
        acumulated += (p_ij[next_i]/sum_p_ij);
    }

    std::vector<float>().swap(p_ij);
    return next_i;
}

std::vector<std::vector<std::vector<std::vector<int>>>> Ants::create_solution(int id_ant,std::vector<std::vector<std::vector<int>>> max_assign_per_session,std::vector<int> list_articles)
{   
    bool show_solution = false;
    bool random_days = true;
    int number_days = max_assign_per_session.size();
    std::vector<std::vector<std::vector<std::vector<int>>>> scheduling;
    std::vector<int> days;
    std::vector<std::vector<int>> block_days(number_days);
    srand(seed);

    //creacion de la solucion
    for(int day=0; day<number_days; day++)
    {
        int number_blocks = max_assign_per_session[day].size();
        std::vector<std::vector<std::vector<int>>> blocks_day;
        for(int block=0; block<number_blocks; block++)
        {
            int number_rooms = max_assign_per_session[day][block].size();
            std::vector<std::vector<int>> rooms_block(number_rooms);
            blocks_day.push_back(rooms_block);
            std::vector<std::vector<int>>().swap(rooms_block);
        }
        scheduling.push_back(blocks_day);
        std::vector<std::vector<std::vector<int>>>().swap(blocks_day);

    }

    int current_article = 0;
    bool roms_left = true;
    int room = 0;
    
    if(show_solution)
    {
        std::cout<<"Solution"<<std::endl;
        std::cout<<"[day,block,room]:[Articles]"<<std::endl; 
    }
    
    while(roms_left)
    {
        roms_left = false;
        for (int day=0; day<number_days; day++)
        {
            int number_blocks = max_assign_per_session[day].size();
            for(int block=0; block<number_blocks; block++)
            {
                int number_rooms = max_assign_per_session[day][block].size();
                if(number_rooms>room)
                {
                    roms_left = true;
                    block_days[day].push_back(block);
                }
            } 
        }

        if(roms_left)
        {   
            for(int day=0; day<number_days; day++)
            {
                if(block_days[day].size()>0)
                {
                    days.push_back(day);
                    //std::cout<<day<<std::endl;
                }
            }
            
            if(random_days)
            {
                int aux_number_days = days.size();
                for(int i=0; i<aux_number_days-1; i++)
                {  
                    int j = i + (rand()%(aux_number_days-i));
                    std::swap(days[i],days[j]);
                }
            }

            auto it = days.begin();  
            while(days.size()>0)
            {
                int day = *it;
                //std::cout<<"day: "<<*it<<std::endl;
                //std::cout<<"number blocks left: "<<block_days[day].size()<<std::endl;
                if(block_days[day].size()>0)
                {
                    int block = block_days[day][0];
                    
                    int number_articles = max_assign_per_session[day][block][room];
                    //std::cout<<number_articles<<std::endl;
                    if(show_solution){std::cout<<day<<","<<block<<","<<room<<" : ";}
                    for(int article=current_article; article<number_articles + current_article; article++)
                    { 
                        int aux_article = list_articles[article];
                        scheduling[day][block][room].push_back(aux_article);
                        if(show_solution){std::cout<<aux_article<<",";}
                    }
                    if(show_solution){std::cout<<std::endl;}

                    current_article+= number_articles;
                    block_days[day].erase(block_days[day].begin());

                    if(block_days[day].size() == 0)
                    {
                        it = days.erase(it);
                    }
                    else
                    {
                        ++it;
                    } 

                    if(it == days.end()){it = days.begin();}
                }
            }
        }
        room+=1;
        //std::cout<<"Room: "<<room<<std::endl;
    }
    
    std::vector<int>().swap(days);
    std::vector<std::vector<int>>().swap(block_days);
    
    return scheduling;

    //std::vector<std::vector<std::vector<std::vector<int>>>>().swap(scheduling);
    
}

void Ants::pheromone_update(std::vector<std::vector<std::vector<std::vector<int>>>> scheduling, float best_solution_quality)
{
    /*Evaporizacion general de la matriz de feromonas*/
    for(int i=0; i<number_articles; i++)
    {
        for(int j=i+1; j<number_articles; j++)
        {    
            float new_pheromona = (float)(1.0-vapor_factor)*pheromone_matrix[i][j];
            
            pheromone_matrix[i][j] = new_pheromona;
            pheromone_matrix[j][i] = new_pheromona;            
        }
    } 

    /*Aumento de feromona para articulos de una misma sesion*/
    int number_days = scheduling.size();
    float solution_pheromona = (float)(c_factor*best_solution_quality);

    for(int day=0; day<number_days; day++)
    {
        int number_blocks = scheduling[day].size();
        for(int block=0; block<number_blocks; block++)
        {
            int number_sessions = scheduling[day][block].size();
            for(int session=0; session<number_sessions; session++)
            {
                int articles_in_session = scheduling[day][block][session].size();
                for(int i=0; i<articles_in_session; i++)
                {
                    int art_1 = scheduling[day][block][session][i];
                    for(int j=i+1; j<articles_in_session; j++)
                    {
                        int art_2 = scheduling[day][block][session][j];
                        pheromone_matrix[art_1][art_2] += solution_pheromona;
                        pheromone_matrix[art_2][art_1] += solution_pheromona;
                    }   
                }
            }
        }
    }

    for(int i=0; i<number_articles; i++)
    {
        for(int j=i+1; j<number_articles; j++)
        {    
            if(pheromone_matrix[i][j]<min_pheromone)
            {
                pheromone_matrix[i][j] = min_pheromone;
                pheromone_matrix[j][i] = min_pheromone;
            }
            if(pheromone_matrix[i][j]>max_pheromone)
            {
                pheromone_matrix[i][j] = max_pheromone;
                pheromone_matrix[j][i] = max_pheromone;
            }       
        }
    } 
}

void Ants::save_solution(std::vector<std::vector<std::vector<std::vector<int>>>> scheduling, float solution_quality)
{

    bool show_top_e = false;
    int size_solution_list = solution_ant.size();

    if(size_solution_list==0){
        solution_ant.push_back(solution_quality);
        ant_solution_scheduling.push_back(scheduling);
    }
    else
    {
        if(solution_quality>solution_ant[0])
        {
            solution_ant.insert(solution_ant.begin(),solution_quality);
            ant_solution_scheduling.insert(ant_solution_scheduling.begin(),scheduling);
        }
        else if(solution_quality<solution_ant[size_solution_list-1]){
            solution_ant.push_back(solution_quality);
            ant_solution_scheduling.push_back(scheduling);
        }
        else{
            int left = 0;
            int right = size_solution_list -1; 
            while (left <= right) {
                int middle = left + (right - left) / 2;
                
                if (solution_ant[middle] > solution_quality){
                    left = middle+1;
                }    
                else if(solution_ant[middle]< solution_quality){
                    right = middle-1;
                }
                else{
                    right = middle-1;
                    left = middle;
                }    
            }
            
            //std::cout<<left<<std::endl;
            
            if(solution_ant[left+1] != solution_quality && solution_ant[left] != solution_quality){
                solution_ant.insert(solution_ant.begin()+left,solution_quality);
                ant_solution_scheduling.insert(ant_solution_scheduling.begin()+left,scheduling);
            }
        }

        int new_size_list = solution_ant.size(); 
        if(new_size_list > e)
        {
            //eliminar las peores soluciones
            solution_ant.pop_back();
            ant_solution_scheduling.pop_back();
        }
    }

    if(show_top_e)
    {
        std::cout<<"show best ants"<<std::endl;
        for (auto it_order = solution_ant.begin(); it_order != solution_ant.end(); ++it_order)
        {
            std::cout << *it_order << " ";
        }
        std::cout<<std::endl;
    }
}

void Ants::reset_ants()
{
    std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>>().swap(ant_solution_scheduling);
    std::vector<float>().swap(solution_ant);    
}

void Ants::pheromone_update_list()
{
    bool sum_e_factor = false;

    /*Evaporizacion general de la matriz de feromonas*/
    for(int i=0; i<number_articles; i++)
    {
        for(int j=i+1; j<number_articles; j++)
        {    
            float new_pheromona = (float)(1.0-vapor_factor)*pheromone_matrix[i][j];
            
            pheromone_matrix[i][j] = new_pheromona;
            pheromone_matrix[j][i] = new_pheromona;            
        }
    }
    
    /*Aumento de feromona para articulos de una misma sesion*/
    int size_solutions_total = ant_solution_scheduling.size();

    std::vector<std::vector<std::vector<std::vector<int>>>> scheduling;

    for(int id_solution=0; id_solution<size_solutions_total; id_solution++)
    {
        scheduling = ant_solution_scheduling[id_solution];
        
        float solution_pheromona = (float)(c_factor*solution_ant[id_solution]);

        if(sum_e_factor)
        {
            solution_pheromona = (solution_pheromona/e);
        }
        
        int number_days = scheduling.size();
        for(int day=0; day<number_days; day++)
        {
            int number_blocks = scheduling[day].size();
            for(int block=0; block<number_blocks; block++)
            {
                int number_sessions = scheduling[day][block].size();
                for(int session=0; session<number_sessions; session++)
                {
                    int articles_in_session = scheduling[day][block][session].size();
                    //std::cout<<day<<","<<block<<","<<session<<","<<articles_in_session<<std::endl;
                    for(int i=0; i<articles_in_session; i++)
                    {
                        int art_1 = scheduling[day][block][session][i];
                        for(int j=i+1; j<articles_in_session; j++)
                        {
                            int art_2 = scheduling[day][block][session][j];
                            pheromone_matrix[art_1][art_2] += solution_pheromona;
                            pheromone_matrix[art_2][art_1] += solution_pheromona;
                            //std::cout<<art_1<<","<<art_2<<","<<solution_pheromona<<std::endl;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::vector<std::vector<std::vector<int>>>>().swap(scheduling);

    for(int i=0; i<number_articles; i++)
    {
        for(int j=i+1; j<number_articles; j++)
        {    
            if(pheromone_matrix[i][j]<min_pheromone)
            {
                pheromone_matrix[i][j] = min_pheromone;
                pheromone_matrix[j][i] = min_pheromone;
            }
            if(pheromone_matrix[i][j]>max_pheromone)
            {
                pheromone_matrix[i][j] = max_pheromone;
                pheromone_matrix[j][i] = max_pheromone;
            }       
        }
    }
}

int Ants::get_number_best_solutions()
{
    return solution_ant.size();
}

std::vector<std::vector<std::vector<std::vector<int>>>> Ants::get_best_solution(int position_solution)
{
    int number_solutions = ant_solution_scheduling.size();
    if(position_solution < number_solutions) 
    {
        return ant_solution_scheduling[position_solution];
    }

    std::vector<std::vector<std::vector<std::vector<int>>>> empty;
    std::cout<<"Erorr best solution position: "<<position_solution<<std::endl;
    return empty;
}

float Ants::get_best_quality_solution(int position_solution)
{
    int number_solutions = solution_ant.size();
    if(position_solution < number_solutions)
    {
        return solution_ant[position_solution];
    }

    float empty = 0;
    std::cout<<"Erorr best solution position: "<<position_solution<<std::endl;
    return empty;
}

void Ants::update_best_solution(Improvement improvement,float new_quality,int id_ant)
{
    bool show_improvement = false;
    
    //std::vector<float> quality_parameters = improvement.get_quality_parameters();
    //float new_quality = calculate_quality_solution(quality_parameters[0],quality_parameters[1],quality_parameters[2]);
    
    if(solution_ant[id_ant] < new_quality)
    {   
        if(show_improvement)
        {
            std::cout<<std::endl;
            std::cout<<"new solution improvement"<<std::endl;
            std::cout<<"Old quality: "<<solution_ant[id_ant]<<std::endl; 
            std::cout<<"New quality: "<<new_quality<<std::endl; 
        }
        
        ant_solution_scheduling[id_ant] = improvement.get_solution_improved();
        solution_ant[id_ant] = new_quality;
    } 
}

int Ants::get_best_solution_found()
{
    int number_solutions = solution_ant.size();
    int position_best_solution = 0;
    float best_quality_found = 0;
    
    for(int pos=0; pos<number_solutions; pos++)
    {
        if(solution_ant[pos] > best_quality_found)
        {
            position_best_solution = pos;
            best_quality_found = solution_ant[pos];
        }
    }

    return position_best_solution;
}

void Ants::update_best_solution_V2(std::vector<std::vector<std::vector<std::vector<int>>>> schedule,float new_quality, int id_ant)
{
    bool show_improvement = false;
    
    if(solution_ant[id_ant] < new_quality)
    {   
        if(show_improvement)
        {
            std::cout<<std::endl;
            std::cout<<"new solution improvement"<<std::endl;
            std::cout<<"Old quality: "<<solution_ant[id_ant]<<std::endl; 
            std::cout<<"New quality: "<<new_quality<<std::endl; 
        }
        
        ant_solution_scheduling[id_ant] = schedule;
        solution_ant[id_ant] = new_quality;
    } 
}

std::vector<float> Ants::get_mean_and_des_std()
{
    int number_articles = pheromone_matrix.size();
    float sum = 0;
    int number_arcs = ((number_articles)*(number_articles-1))/2 ;

    for(int i=0; i<number_articles; i++)
    {
        for(int j=i+1; j<number_articles; j++)
        {
            sum +=  pheromone_matrix[i][j];
        }
    }

    float mean  = (float) (sum/number_arcs);

    float des_std = 0.0;

    for(int i=0; i<number_articles; i++)
    {
        for(int j=i+1; j<number_articles; j++)
        {
            des_std += pow(pheromone_matrix[i][j]-mean,2);
        }
    } 

    des_std = (float) (des_std/number_arcs);
    des_std = pow(des_std,0.5);

    std::vector<float> params_matrix = {mean, des_std};
    return params_matrix;  
}
