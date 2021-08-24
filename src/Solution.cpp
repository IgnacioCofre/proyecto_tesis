#include "../includes/solutions.h"

void Solutions::read_solution(const char * solution_path)
{
    FILE * pFile;
    pFile = fopen (solution_path,"r");

    //lectura del archivo
    if(!pFile)
    {
	    std::cerr << "Problem while reading solution" << std::endl;
	    exit(EXIT_FAILURE);
    }
    //numero de dias
    if(fscanf(pFile, "%d %d", &number_days, &number_articles) != 2)
    {
        std::cerr << "Problem while reading number of days and articles" << std::endl;
        exit(EXIT_FAILURE);
    }
    //vector de vectores que guarda la asignacion de cada articulo en la calendarizacion
    std::vector<std::vector<int>> new_article_asignation(number_articles,std::vector<int>(3,-1));

    //iteracion sobre los dias de la calendarizacion
    
    for(int day = 0; day < number_days; day++)
    {
        std::vector<int> new_data_day(2);
        if(fscanf(pFile, "%d %d", &new_data_day[0], &new_data_day[1]) != 2)
        {
            std::cerr << "Problem while reading number of blocks and number of sessions" << std::endl;
            exit(EXIT_FAILURE);
        }

        int number_blocks = new_data_day[0];
        int number_sessions = new_data_day[1];
        data_day.push_back(new_data_day);

        std::vector<std::vector<std::vector<int>>> new_day(number_blocks);
        //iteracion sobre las sessiones que se realizan en un dia 
        for(int session = 0; session<number_sessions; session++)
        {
            int block_session;
            int number_articles_per_sessison;
            if(fscanf(pFile, "%d %d", &block_session, &number_articles_per_sessison) != 2)
            {
                std::cerr << "Problem while reading article of blocks" << std::endl;
                exit(EXIT_FAILURE);
            }

            //std::cout << block_session << std::endl;
            //std::cout << number_articles_per_sessison << std::endl;
            std::vector<int> articles_in_session(number_articles_per_sessison);
            //iteracion sobre los articulos que se asignaron en una session 
            for(int i = 0; i<number_articles_per_sessison; i++)
            {
                if(fscanf(pFile, "%d", &articles_in_session[i]) != 1)
                {
                    std::cerr << "Problem while reading article id" << std::endl;
                    exit(EXIT_FAILURE);
                }
                //std::cout << day << " " << block_session-1 << " " << session <<std::endl;
                else
                {
                    if(new_article_asignation[articles_in_session[i]][0] == -1)
                    {
                        new_article_asignation[articles_in_session[i]] = {day, block_session-1, session};
                    }

                    else
                    {
                        std::cerr << "Problem of output, same article apears in two diferent sessions: " << articles_in_session[i] <<std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
            new_day[block_session-1].push_back(articles_in_session);

        }
        scheduling.push_back(new_day);
    }

    article_asignation = new_article_asignation; 
    fclose(pFile);

}

std::vector<std::vector<std::vector<std::vector<int>>>> Solutions::get_scheduling()
{
    return scheduling;
}

std::vector<int> Solutions::get_article_asignation(int id_article)
{
    /*
    std::cout<< "Article asignation" <<std::endl;
    std::cout<< article_asignation[id_article][0] <<std::endl;
    std::cout<< article_asignation[id_article][1] <<std::endl;
    std::cout<< article_asignation[id_article][2] <<std::endl;
    */
    return article_asignation[id_article];
}

int Solutions::get_number_articles()
{
    return number_articles;
}

int Solutions::get_number_days()
{
    return number_days;
}

void Solutions::show_solution()
{
    std::cout << "Asignacion of articles [Day,Block,Session]" << std::endl;
    for(int i=0; i<number_articles; i++)
    {
        std::cout << article_asignation[i][0] << "," << article_asignation[i][1] << ","<< article_asignation[i][2] <<std::endl;
    }
}