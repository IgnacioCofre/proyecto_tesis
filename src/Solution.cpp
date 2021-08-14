#include "../includes/class.h"

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
    if(fscanf(pFile, "%d", &number_days) != 1)
    {
        std::cerr << "Problem while reading number of days" << std::endl;
        exit(EXIT_FAILURE);
    }
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

            }
            new_day[block_session-1].push_back(articles_in_session);

        }
        scheduling.push_back(new_day);
    } 
    fclose(pFile);

}

std::vector<std::vector<std::vector<std::vector<int>>>> Solutions::get_scheduling()
{
    return scheduling;
}