#include "../includes/class.h"

void Data::read_input_file(const char * input_path)
{
    
    FILE * pFile;
    pFile = fopen (input_path,"r");

    //lectura del archivo
    if(!pFile)
    {
	    std::cerr << "Problem while reading instance" << std::endl;
	    exit(EXIT_FAILURE);
    }

    //numero de articulos
    if(fscanf(pFile, "%d", &number_articles) != 1)
    {
        std::cerr << "Problem while reading number of articles" << std::endl;
        exit(EXIT_FAILURE);
    }

    //matriz de similaridad
    for (int _ = 0; _ < number_articles; _++)
    {
        std::vector <int> new_row(number_articles);

        for(int i = 0; i < number_articles; i++)
        {
            if(fscanf(pFile, "%d", &new_row[i]) != 1){
                std::cerr << "Problem while reading similarity matrix" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        similarity_matrix.push_back(new_row);

    }

    //numero de tipo de sesiones
    if(fscanf(pFile, "%d", &number_type_sessions) != 1)
    {
        std::cerr << "Problem while reading session number" << std::endl;
        exit(EXIT_FAILURE);
    }
    //informacio del tipo de sesiones
    for(int i = 0; i<number_type_sessions; i++)
    {
        int input_per_session = 3;
        std::vector<int> new_session_data(input_per_session);
        if(fscanf(pFile, "%d %d %d", &new_session_data[0],&new_session_data[1],&new_session_data[2])!= input_per_session)
        {
            std::cerr << "Problem while reading session tipe data" << std::endl;
            exit(EXIT_FAILURE);
        } 
    }
    //numero de dias
    if(fscanf(pFile, "%d", &number_days) != 1)
    {
        std::cerr << "Problem while reading number days" << std::endl;
        exit(EXIT_FAILURE);
    }

    for(int day = 0; day<number_days; day++)
    {
        std::vector<int> new_data_log(2);    
        //&new_data_log[0] : numero de bloques del dia
        //&new_data_log[1] : numero de sesiones del dia
        if(fscanf(pFile, "%d %d", &new_data_log[0], &new_data_log[1]) != 2)
        {
            std::cerr << "Problem while reading session tipe data" << std::endl;
            exit(EXIT_FAILURE);
        }
        data_days.push_back(new_data_log);

        std::vector<std::vector<std::vector<int>>> new_data_day;
        for(int session = 0; session < new_data_log[1]; session++)
        {
            std::vector<int> new_session(2);
            //&new_data_day_block[0] : bloque en que se realiza la sesion
            //&new_data_day_block[1] : tamaño maximo de dicha sesion
            if(fscanf(pFile, "%d %d", &new_session[0], &new_session[1]) != 2)
            {
                std::cerr << "Problem while reading data block" << std::endl;
                exit(EXIT_FAILURE);
            } 

            session_data.push_back(new_session);
        }

        new_data_day.push_back(session_data);
        
    }
    //numero de articulos y numero de topicos
    int aux;
    if(fscanf(pFile, "%d %d", &aux , &number_topics) != 2)
    {
        std::cerr << "Problem while reading number topics" << std::endl;
        exit(EXIT_FAILURE);
    }
    //topicos por articulo
    for(int i = 0; i < number_articles; i++)
    {
        int number_article_topics;
        if(fscanf(pFile, "%d",&number_article_topics) != 1)
        {
            std::cerr << "Problem while reading number of topics per article" << std::endl;
            exit(EXIT_FAILURE);
        } 
        number_topics_articles.push_back(number_article_topics);

        std::vector<int> topics_article(number_article_topics);
        for(int j = 0; j<number_article_topics; j++)
        {   
            if(fscanf(pFile, "%d",&topics_article[j]) != 1){
                std::cerr << "Problem while reading topics per article" << std::endl;
                exit(EXIT_FAILURE);
            }    
        }
        articles_topics.push_back(topics_article);

    }
    //numero de articulos y numero de autores
    int aux2;
    if(fscanf(pFile, "%d %d", &aux2 , &number_authors) != 2)
    {
        std::cerr << "Problem while reading number authors" << std::endl;
        exit(EXIT_FAILURE);
    }
    //autores por articulo
    for(int i = 0; i < number_articles; i++)
    {
        int new_number_article_authors;
        if(fscanf(pFile, "%d",&new_number_article_authors) != 1)
        {
            std::cerr << "Problem while reading number of authors per article" << std::endl;
            exit(EXIT_FAILURE);
        } 
        number_articles_authors.push_back(new_number_article_authors);

        std::vector<int> new_articles_authors(new_number_article_authors);
        for(int j = 0; j<new_number_article_authors; j++)
        {   
            if(fscanf(pFile, "%d",&new_articles_authors[j]) != 1){
                std::cerr << "Problem while reading author per article" << std::endl;
                exit(EXIT_FAILURE);
            }    
        }
        articles_authors.push_back(new_articles_authors);

    }
    fclose(pFile);
}

int Data::get_similarity(int id_article_1, int id_article_2)
{
    if((id_article_1 <number_articles) & (id_article_2<number_articles))
    {
        return similarity_matrix[id_article_1][id_article_2];
    }

    std::cerr << "Problem while reading number of article" << std::endl;
    exit(EXIT_FAILURE);
}

void Data::show_data()
{
    std::cout << number_articles << std::endl;

    for(int i = 0; i<number_articles; i++)
    {
        for(int j = 0; j<number_articles; j++)
        {
            std::cout << similarity_matrix[i][j] << " ";
        }

        std::cout << std::endl; 
    }

    std::cout << number_type_sessions << std::endl;
    std::cout << number_days << std::endl;
    std::cout << number_topics << std::endl;
    std::cout << number_authors << std::endl;
}

int Data::get_number_days()
{
    return number_days;
}

int Data::get_number_articles()
{
    return number_articles;
}