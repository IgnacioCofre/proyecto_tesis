#include "../includes/improvement.h"

//Improvement(solution, quality, limit_iteration, gamma, epsilon, articles, topics, authors)
Improvement::Improvement(std::vector<std::vector<std::vector<std::vector<int>>>> current_solution, float s_q, float g, float ep, Articles articles, Topics topics, Authors authors)
{
    solution_to_improve = current_solution;
    current_solution_quality = s_q;
    gamma = g;
    epsilon = ep;

    int number_days = solution_to_improve.size();
    int number_articles = articles.get_number_articles();
    int number_topics = topics.get_number_topics();
    int number_authors = authors.get_number_authors();

    /*Ubicacion de los articulos y beneficio total de la solucion*/
    std::vector<std::vector<int>> article_ubication_aux(number_articles, {-1, -1, -1, -1});
    std::vector<std::vector<std::vector<int>>> benefit_session_aux;
    std::vector<int> authors_conflicts_aux(authors.get_number_authors(), 0);
    std::vector<std::vector<int>> number_article_per_day_by_topic_aux(number_days, std::vector<int>(number_topics, 0));

    for (int day = 0; day < number_days; day++)
    {
        int number_blocks = solution_to_improve[day].size();
        std::vector<std::vector<int>> block_in_days;
        //std::cout<<"day: "<<day<<std::endl;
        for (int block = 0; block < number_blocks; block++)
        {
            int number_sessions = solution_to_improve[day][block].size();
            std::vector<int> sessions_in_block;
            //aux_authors_conflicts_per_block[day].push_back(0);

            //std::cout<<"block: "<<block<<std::endl;
            for (int session = 0; session < number_sessions; session++)
            {
                int benefit_accumulated_in_session = 0;
                int n_articles_session = solution_to_improve[day][block][session].size();

                //std::cout<<"session: "<<session<<std::endl;
                for (int iter_article = 0; iter_article < n_articles_session; iter_article++)
                {
                    //std::cout<<"iter_article: "<<iter_article<<std::endl;
                    int id_article = solution_to_improve[day][block][session][iter_article];
                    //std::cout<<"id_article: "<<id_article<<std::endl;

                    //ubicacion del articulo en la calendarizacion
                    article_ubication_aux[id_article] = {day, block, session, iter_article};

                    //conteo de articulos por topico por dia
                    std::vector<int> list_of_topics = topics.get_article_topics(id_article);
                    for (auto id_topic : list_of_topics)
                    {
                        //std::cout<<"id_topic: "<<id_topic<<std::endl;
                        number_article_per_day_by_topic_aux[day][id_topic] += 1;
                    }
                    std::vector<int>().swap(list_of_topics);

                    //conteo de beneficio por session
                    for (int iter_article_2 = iter_article + 1; iter_article_2 < n_articles_session; iter_article_2++)
                    {
                        int id_article_2 = solution_to_improve[day][block][session][iter_article_2];
                        int benefit_pair_articles = articles.get_similarity(id_article, id_article_2);
                        benefit_accumulated_in_session += benefit_pair_articles;
                        total_benefit += benefit_pair_articles;
                    }
                    //std::cout<<"id_article end: "<<id_article<<std::endl;
                }

                //std::cout<<"session end: "<<session<<std::endl;
                sessions_in_block.push_back(benefit_accumulated_in_session);
            }
            block_in_days.push_back(sessions_in_block);
            std::vector<int>().swap(sessions_in_block);
        }
        benefit_session_aux.push_back(block_in_days);
        std::vector<std::vector<int>>().swap(block_in_days);
    }

    number_article_per_day_by_topic = number_article_per_day_by_topic_aux;
    benefit_session = benefit_session_aux;
    article_ubication = article_ubication_aux;

    /*Cantidad de topes de horario*/
    for (int author = 0; author < number_authors; author++)
    {
        std::vector<int> list_articles = authors.get_author_articles(author);
        int n_articles = list_articles.size();

        if (n_articles > 1)
        {
            for (int i = 0; i < n_articles; i++)
            {
                int id_article_1 = list_articles[i];
                int day_article_1 = article_ubication_aux[id_article_1][0];
                int block_article_1 = article_ubication_aux[id_article_1][1];
                int sesison_article_1 = article_ubication_aux[id_article_1][2];

                for (int j = i + 1; j < n_articles; j++)
                {
                    int id_article_2 = list_articles[j];
                    int day_article_2 = article_ubication_aux[id_article_2][0];
                    int block_article_2 = article_ubication_aux[id_article_2][1];
                    int sesison_article_2 = article_ubication_aux[id_article_2][2];

                    //se verifica primero que los dias y el bloque de dos articulos de un mismo autor sean iguales
                    if ((day_article_1 == day_article_2) & (block_article_1 == block_article_2))
                    {
                        //si las sesiones son diferentes no se cumplen con las restricciones
                        if (sesison_article_1 != sesison_article_2)
                        {
                            authors_conflicts_aux[author] += 1;
                            number_autor_conflicts += 1;
                        }
                    }
                }
            }
        }

        std::vector<int>().swap(list_articles);
    }

    authors_conflicts = authors_conflicts_aux;

    /*Cantidad de articulos que sobrepasan el maximo diario por topico*/
    float new_number_topics_conflics = 0.0;
    for (int day = 0; day < number_days; day++)
    {
        for (int id_topic = 0; id_topic < number_topics; id_topic++)
        {
            int number_articles_in_day_by_topic = number_article_per_day_by_topic[day][id_topic];
            if (number_articles_in_day_by_topic > topics.get_max_per_day(id_topic))
            {
                float over_max_topics = number_articles_in_day_by_topic - topics.get_max_per_day(id_topic);
                new_number_topics_conflics += (float)(over_max_topics / topics.get_max_per_day(id_topic));
            }
        }
    }

    number_topics_conflics = new_number_topics_conflics;
}

void Improvement::swap_articles(int id_article_1, int id_article_2, Articles articles, Topics topics, Authors authors)
{
    bool show_changes = false;
    //bool save_if_better = true;

    std::vector<std::vector<std::vector<std::vector<int>>>> new_solution = solution_to_improve;
    std::vector<std::vector<std::vector<int>>> new_benefit_session = benefit_session;
    std::vector<std::vector<int>> new_article_ubication = article_ubication;
    std::vector<int> new_author_conflics = authors_conflicts;
    std::vector<std::vector<int>> new_number_article_per_day_by_topic = number_article_per_day_by_topic;

    std::vector<int> aux_vect = {-1, -1, -1, -1};
    /*Revisa que los articulos hallan sido previamente instanciados en la solucion por modificar*/
    if ((article_ubication[id_article_1] != aux_vect) && (article_ubication[id_article_2] != aux_vect))
    {
        int day_1 = article_ubication[id_article_1][0];
        int block_1 = article_ubication[id_article_1][1];
        int room_1 = article_ubication[id_article_1][2];
        int pos_1 = article_ubication[id_article_1][3];

        int day_2 = article_ubication[id_article_2][0];
        int block_2 = article_ubication[id_article_2][1];
        int room_2 = article_ubication[id_article_2][2];
        int pos_2 = article_ubication[id_article_2][3];

        //Intercambio de articulos
        new_solution[day_1][block_1][room_1][pos_1] = id_article_2;
        new_solution[day_2][block_2][room_2][pos_2] = id_article_1;

        new_article_ubication[id_article_1] = {day_2, block_2, room_2, pos_2};
        new_article_ubication[id_article_2] = {day_1, block_1, room_1, pos_1};

        //calculo del nuevo beneficio generado para la primera sesion
        new_benefit_session[day_1][block_1][room_1] = 0;
        std::vector<int> articles_in_session_1 = new_solution[day_1][block_1][room_1];
        int number_articles_session_1 = articles_in_session_1.size();
        for (int i = 0; i < number_articles_session_1; i++)
        {
            int article_1 = articles_in_session_1[i];
            for (int j = i + 1; j < number_articles_session_1; j++)
            {
                int article_2 = articles_in_session_1[j];
                new_benefit_session[day_1][block_1][room_1] += articles.get_similarity(article_1, article_2);
            }
        }

        //calculo del nuevo beneficio generado para la segunda sesion
        new_benefit_session[day_2][block_2][room_2] = 0;
        std::vector<int> articles_in_session_2 = new_solution[day_2][block_2][room_2];
        int number_articles_session_2 = articles_in_session_2.size();
        for (int i = 0; i < number_articles_session_2; i++)
        {
            int article_1 = articles_in_session_2[i];
            for (int j = i + 1; j < number_articles_session_2; j++)
            {
                int article_2 = articles_in_session_2[j];
                new_benefit_session[day_2][block_2][room_2] += articles.get_similarity(article_1, article_2);
            }
        }

        //Calculo del beneficio total despues del intercambio de articulos
        int new_total_benefit = 0;

        int number_days = new_solution.size();
        for (int day = 0; day < number_days; day++)
        {
            int number_blocks = new_solution[day].size();
            for (int block = 0; block < number_blocks; block++)
            {
                int number_sessions = new_solution[day][block].size();
                for (int session = 0; session < number_sessions; session++)
                {
                    new_total_benefit += new_benefit_session[day][block][session];
                }
            }
        }

        //Calculo de nuevos conflictos de topes de horario despues del intercambio
        for (auto id_article : {id_article_1, id_article_2})
        {
            std::vector<int> article_authors = authors.get_article_authors(id_article);
            for (auto author : article_authors)
            {
                new_author_conflics[author] = 0;
                //int new_conflics = 0;
                std::vector<int> author_articles = authors.get_author_articles(author);
                int number_author_articles = author_articles.size();
                if (number_author_articles > 1)
                {
                    for (int i = 0; i < number_author_articles; i++)
                    {
                        int id_art_1 = author_articles[i];
                        int day_article_1 = new_article_ubication[id_art_1][0];
                        int block_article_1 = new_article_ubication[id_art_1][1];
                        int sesison_article_1 = new_article_ubication[id_art_1][2];

                        for (int j = i + 1; j < number_author_articles; j++)
                        {
                            int id_art_2 = author_articles[j];
                            int day_article_2 = new_article_ubication[id_art_2][0];
                            int block_article_2 = new_article_ubication[id_art_2][1];
                            int sesison_article_2 = new_article_ubication[id_art_2][2];

                            //se verifica primero que los dias y el bloque de dos articulos de un mismo autor sean iguales
                            if ((day_article_1 == day_article_2) & (block_article_1 == block_article_2))
                            {
                                //si las sesiones son diferentes no se cumplen con las restricciones
                                if (sesison_article_1 != sesison_article_2)
                                {
                                    new_author_conflics[author] += 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        int new_number_author_conflicts = 0;

        for (auto n_author_conflicts : new_author_conflics)
        {
            new_number_author_conflicts += n_author_conflicts;
        }

        //calculo del maximo de topicos por dia
        //solo si los articulos son de dias distintos se realiza un nuevo calculo
        if (day_1 != day_2)
        {
            //cambio de la cantidad de los topicos respecto al primer articulo
            std::vector<int> list_of_topics = topics.get_article_topics(id_article_1);
            for (auto id_topic : list_of_topics)
            {
                new_number_article_per_day_by_topic[day_1][id_topic] -= 1;
                new_number_article_per_day_by_topic[day_2][id_topic] += 1;
            }

            //cambio de la cantidad de los topicos respecto al segundo articulo
            list_of_topics = topics.get_article_topics(id_article_2);
            for (auto id_topic : list_of_topics)
            {
                new_number_article_per_day_by_topic[day_2][id_topic] -= 1;
                new_number_article_per_day_by_topic[day_1][id_topic] += 1;
            }
            std::vector<int>().swap(list_of_topics);
        }

        //cantida de articulos que pasan el maximo
        int number_topics = topics.get_number_topics();
        float new_number_topics_conflics = 0.0;
        for (int day = 0; day < number_days; day++)
        {
            for (int id_topic = 0; id_topic < number_topics; id_topic++)
            {
                int number_articles_in_day_by_topic = new_number_article_per_day_by_topic[day][id_topic];
                if (number_articles_in_day_by_topic > topics.get_max_per_day(id_topic))
                {
                    float over_max_topics = number_articles_in_day_by_topic - topics.get_max_per_day(id_topic);
                    new_number_topics_conflics += (float)over_max_topics / topics.get_max_per_day(id_topic);
                }
            }
        }

        if (show_changes)
        {
            std::cout << std::endl;
            std::cout << "Beneficio antes del cambio: " << total_benefit << std::endl;
            std::cout << "N° conflictos tope horario: " << number_autor_conflicts << std::endl;
            std::cout << "N° conflictos topicos:      " << number_topics_conflics << std::endl;
            std::cout << "Beneficio despues:          " << new_total_benefit << std::endl;
            std::cout << "N° conflictos tope horario: " << new_number_author_conflicts << std::endl;
            std::cout << "N° conflictos topicos:      " << new_number_topics_conflics << std::endl;
        }

        if ((new_total_benefit > total_benefit) || (new_number_author_conflicts < number_autor_conflicts))
        {
            //std::cout<<"New better solution found"<<std::endl;
            solution_to_improve = new_solution;
            article_ubication = new_article_ubication;

            total_benefit = new_total_benefit;
            benefit_session = new_benefit_session;
            authors_conflicts = new_author_conflics;

            number_article_per_day_by_topic = new_number_article_per_day_by_topic;

            number_autor_conflicts = new_number_author_conflicts;
            number_topics_conflics = new_number_topics_conflics;
            //current_solution_quality = new_current_solution_quality
        }
    }
    else
    {
        std::cout << "Error id articles, id_article_1: " << id_article_1 << " id_article_2: " << id_article_2 << std::endl;
    }

    std::vector<std::vector<std::vector<std::vector<int>>>>().swap(new_solution);
    std::vector<std::vector<std::vector<int>>>().swap(new_benefit_session);
    std::vector<std::vector<int>>().swap(new_article_ubication);
    std::vector<int>().swap(new_author_conflics);
    std::vector<std::vector<int>>().swap(new_number_article_per_day_by_topic);
}

void Improvement::show_data()
{
    //article_ubication[id_article] = [day,block,room] of article assignation in solution_to_improve
    int number_articles = article_ubication.size();
    std::cout << "Article ubication [id_article: day,block,room,pos]" << std::endl;
    for (int art = 0; art < number_articles; art++)
    {
        std::cout << art << ":" << article_ubication[art][0] << "," << article_ubication[art][1] << "," << article_ubication[art][2] << "," << article_ubication[art][3] << std::endl;
    }

    //benefit_session[day,block,room] = int benefit of session
    std::cout << "Benefit session [day,block,room:benefit]" << std::endl;
    int new_benefit = 0;
    int number_days = benefit_session.size();
    for (int day = 0; day < number_days; day++)
    {
        int number_blocks = benefit_session[day].size();
        for (int block = 0; block < number_blocks; block++)
        {
            int number_rooms = benefit_session[day][block].size();
            for (int room = 0; room < number_rooms; room++)
            {
                std::cout << "[" << day << "," << block << "," << room << "]"
                          << ":" << benefit_session[day][block][room] << std::endl;
                new_benefit += benefit_session[day][block][room];
            }
        }
    }
    std::cout << "Total benefit solution in change: " << new_benefit << std::endl;

    //authors_conflicts[id_author] = int number of conflicts of author
    std::cout << "Conflics per author [id_author:number_conflicts]" << std::endl;
    int number_authors = authors_conflicts.size();
    for (int author = 0; author < number_authors; author++)
    {
        int number_conflicts = authors_conflicts[author];
        std::cout << author << ":" << number_conflicts << std::endl;
    }

    //number_article_per_day_by_topic[day][id_topic] = int number of article in the day that contains id_topic
    number_days = number_article_per_day_by_topic.size();

    std::cout << "Number article per day by topic [day,id_topic:number articles with topic]" << std::endl;
    for (int day = 0; day < number_days; day++)
    {
        int number_topics = number_article_per_day_by_topic[day].size();
        for (int topic = 0; topic < number_topics; topic++)
        {
            std::cout << day << "," << topic << ":" << number_article_per_day_by_topic[day][topic] << std::endl;
        }
    }
}

bool Improvement::in_diferent_session(int id_article_1, int id_article_2)
{
    std::vector<int> a_u_1 = article_ubication[id_article_1];
    std::vector<int> a_u_2 = article_ubication[id_article_2];

    if ((a_u_1[0] == a_u_2[0]) && (a_u_1[1] == a_u_2[1]) && (a_u_1[2] == a_u_2[2]))
    {
        return false;
    }

    return true;
}

int Improvement::select_article_in_diferent_session(int id_article)
{
    std::vector<int> a_u_1 = article_ubication[id_article];

    std::random_device rd;
    std::mt19937 gen(rd());

    int number_articles = article_ubication.size();
    std::uniform_int_distribution<> distr_articles(0, number_articles - 1);
    int new_article = distr_articles(gen);
    std::vector<int> a_u_2 = article_ubication[new_article];

    //se selecciona otro articulo al azar hasta que se encuentre uno que no este en la misma session que id_article
    while ((a_u_1[0] == a_u_2[0]) && (a_u_1[1] == a_u_2[1]) && (a_u_1[2] == a_u_2[2]))
    {
        new_article = distr_articles(gen);
        a_u_2 = article_ubication[new_article];
    }

    return new_article;
}

std::vector<std::vector<std::vector<std::vector<int>>>> Improvement::get_solution_improved()
{
    return solution_to_improve;
}

int Improvement::get_benefit_solution_improved()
{
    return total_benefit;
}

int Improvement::get_number_autor_conflicts()
{
    return number_autor_conflicts;
}

float Improvement::get_number_topics_conflicts()
{
    return number_topics_conflics;
}

int Improvement::swap_articles_V2(int id_article_1, int id_article_2, Articles articles, Topics topics, Authors authors)
{
    bool show_changes = false;
    //no hay mejora de la solucion
    int improvement_case = 1;

    std::vector<std::vector<int>> new_article_ubication = article_ubication;
    std::vector<int> new_author_conflics = authors_conflicts;

    std::vector<int> aux_vect = {-1, -1, -1, -1};

    /*Revisa que los articulos hallan sido previamente instanciados en la solucion por modificar*/
    if ((article_ubication[id_article_1] != aux_vect) && (article_ubication[id_article_2] != aux_vect))
    {
        int number_days = solution_to_improve.size();

        int day_1 = article_ubication[id_article_1][0];
        int block_1 = article_ubication[id_article_1][1];
        int room_1 = article_ubication[id_article_1][2];
        int pos_1 = article_ubication[id_article_1][3];

        int day_2 = article_ubication[id_article_2][0];
        int block_2 = article_ubication[id_article_2][1];
        int room_2 = article_ubication[id_article_2][2];
        int pos_2 = article_ubication[id_article_2][3];

        //new_solution[day_1][block_1][room_1][pos_1] = id_article_2;
        //new_solution[day_2][block_2][room_2][pos_2] = id_article_1;

        //Evaluacion del intercambio de articulos
        new_article_ubication[id_article_1] = {day_2, block_2, room_2, pos_2};
        new_article_ubication[id_article_2] = {day_1, block_1, room_1, pos_1};

        //Calculo de nuevos conflictos de topes de horario despues del intercambio
        for (auto id_article : {id_article_1, id_article_2})
        {
            std::vector<int> article_authors = authors.get_article_authors(id_article);
            for (auto author : article_authors)
            {
                new_author_conflics[author] = 0;
                //int new_conflics = 0;
                std::vector<int> author_articles = authors.get_author_articles(author);
                int number_author_articles = author_articles.size();
                if (number_author_articles > 1)
                {
                    for (int i = 0; i < number_author_articles; i++)
                    {
                        int id_art_1 = author_articles[i];
                        int day_article_1 = new_article_ubication[id_art_1][0];
                        int block_article_1 = new_article_ubication[id_art_1][1];
                        int sesison_article_1 = new_article_ubication[id_art_1][2];

                        for (int j = i + 1; j < number_author_articles; j++)
                        {
                            int id_art_2 = author_articles[j];
                            int day_article_2 = new_article_ubication[id_art_2][0];
                            int block_article_2 = new_article_ubication[id_art_2][1];
                            int sesison_article_2 = new_article_ubication[id_art_2][2];

                            //se verifica primero que los dias y el bloque de dos articulos de un mismo autor sean iguales
                            if ((day_article_1 == day_article_2) & (block_article_1 == block_article_2))
                            {
                                //si las sesiones son diferentes no se cumplen con las restricciones
                                if (sesison_article_1 != sesison_article_2)
                                {
                                    new_author_conflics[author] += 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        int new_number_author_conflicts = 0;

        for (auto n_author_conflicts : new_author_conflics)
        {
            new_number_author_conflicts += n_author_conflicts;
        }

        /*Seleccion lexicografica de las soluciones*/

        //si la solucion disminulle la
        if (new_number_author_conflicts <= number_autor_conflicts)
        {
            //calculo del maximo de topicos por dia
            //solo si los articulos son de dias distintos se realiza un nuevo calculo

            std::vector<std::vector<int>> new_number_article_per_day_by_topic = number_article_per_day_by_topic;
            if (day_1 != day_2)
            {
                //cambio de la cantidad de los topicos respecto al primer articulo
                std::vector<int> list_of_topics = topics.get_article_topics(id_article_1);
                for (auto id_topic : list_of_topics)
                {
                    new_number_article_per_day_by_topic[day_1][id_topic] -= 1;
                    new_number_article_per_day_by_topic[day_2][id_topic] += 1;
                }

                //cambio de la cantidad de los topicos respecto al segundo articulo
                list_of_topics = topics.get_article_topics(id_article_2);
                for (auto id_topic : list_of_topics)
                {
                    new_number_article_per_day_by_topic[day_2][id_topic] -= 1;
                    new_number_article_per_day_by_topic[day_1][id_topic] += 1;
                }
                std::vector<int>().swap(list_of_topics);
            }

            //cantida de articulos que pasan el maximo
            int number_topics = topics.get_number_topics();
            float new_number_topics_conflics = 0.0;
            for (int day = 0; day < number_days; day++)
            {
                for (int id_topic = 0; id_topic < number_topics; id_topic++)
                {
                    int number_articles_in_day_by_topic = new_number_article_per_day_by_topic[day][id_topic];
                    if (number_articles_in_day_by_topic > topics.get_max_per_day(id_topic))
                    {
                        float over_max_topics = number_articles_in_day_by_topic - topics.get_max_per_day(id_topic);
                        new_number_topics_conflics += (float)over_max_topics / topics.get_max_per_day(id_topic);
                    }
                }
            }

            //si la solucion presenta menos o iguales conflictos de topicos se considera para la siguiente etapa
            if (new_number_topics_conflics <= number_topics_conflics)
            {
                /*
                std::vector<std::vector<std::vector<std::vector<int>>>> new_solution = solution_to_improve;
                new_solution[day_1][block_1][room_1][pos_1] = id_article_2;
                new_solution[day_2][block_2][room_2][pos_2] = id_article_1;
                */

                std::vector<std::vector<std::vector<int>>> new_benefit_session = benefit_session;

                //calculo del nuevo beneficio generado para la primera sesion
                new_benefit_session[day_1][block_1][room_1] = 0;

                std::vector<int> articles_in_session_1 = solution_to_improve[day_1][block_1][room_1];
                articles_in_session_1[pos_1] = id_article_2;

                int number_articles_session_1 = articles_in_session_1.size();
                for (int i = 0; i < number_articles_session_1; i++)
                {
                    int article_1 = articles_in_session_1[i];
                    for (int j = i + 1; j < number_articles_session_1; j++)
                    {
                        int article_2 = articles_in_session_1[j];
                        new_benefit_session[day_1][block_1][room_1] += articles.get_similarity(article_1, article_2);
                    }
                }

                //calculo del nuevo beneficio generado para la segunda sesion
                new_benefit_session[day_2][block_2][room_2] = 0;

                std::vector<int> articles_in_session_2 = solution_to_improve[day_2][block_2][room_2];
                articles_in_session_2[pos_2] = id_article_1;

                int number_articles_session_2 = articles_in_session_2.size();
                for (int i = 0; i < number_articles_session_2; i++)
                {
                    int article_1 = articles_in_session_2[i];
                    for (int j = i + 1; j < number_articles_session_2; j++)
                    {
                        int article_2 = articles_in_session_2[j];
                        new_benefit_session[day_2][block_2][room_2] += articles.get_similarity(article_1, article_2);
                    }
                }

                //Calculo del beneficio total despues del intercambio de articulos
                int new_total_benefit = 0;
                for (int day = 0; day < number_days; day++)
                {
                    int number_blocks = new_benefit_session[day].size();
                    for (int block = 0; block < number_blocks; block++)
                    {
                        int number_sessions = new_benefit_session[day][block].size();
                        for (int session = 0; session < number_sessions; session++)
                        {
                            new_total_benefit += new_benefit_session[day][block][session];
                        }
                    }
                }

                //en caso de que el beneficio sea mejor producto al intercambio
                if((new_total_benefit > total_benefit) || (new_number_author_conflicts < number_autor_conflicts))
                //if (new_total_benefit > total_benefit)
                {
                    if (show_changes)
                    {
                        std::cout << std::endl;
                        std::cout << "Beneficio antes del cambio: " << total_benefit << std::endl;
                        std::cout << "N° conflictos tope horario: " << number_autor_conflicts << std::endl;
                        std::cout << "N° conflictos topicos:      " << number_topics_conflics << std::endl;
                        std::cout << "Beneficio despues:          " << new_total_benefit << std::endl;
                        std::cout << "N° conflictos tope horario: " << new_number_author_conflicts << std::endl;
                        std::cout << "N° conflictos topicos:      " << new_number_topics_conflics << std::endl;
                    }

                    //Se realiza el intercambio
                    std::vector<std::vector<std::vector<std::vector<int>>>> new_solution = solution_to_improve;
                    new_solution[day_1][block_1][room_1][pos_1] = id_article_2;
                    new_solution[day_2][block_2][room_2][pos_2] = id_article_1;

                    solution_to_improve = new_solution;
                    article_ubication = new_article_ubication;

                    total_benefit = new_total_benefit;
                    benefit_session = new_benefit_session;
                    authors_conflicts = new_author_conflics;

                    number_article_per_day_by_topic = new_number_article_per_day_by_topic;

                    number_autor_conflicts = new_number_author_conflicts;
                    number_topics_conflics = new_number_topics_conflics;

                    //se encontro mejora
                    improvement_case = 0;

                    std::vector<std::vector<std::vector<std::vector<int>>>>().swap(new_solution);
                }
                std::vector<std::vector<std::vector<int>>>().swap(new_benefit_session);
            }

            std::vector<std::vector<int>>().swap(new_number_article_per_day_by_topic);
        }
    }
    else
    {
        std::cout << "Error id articles, id_article_1: " << id_article_1 << " id_article_2: " << id_article_2 << std::endl;
    }

    std::vector<std::vector<int>>().swap(new_article_ubication);
    std::vector<int>().swap(new_author_conflics);

    return improvement_case;
}

std::vector<int> Improvement::get_articles_author_conflicts(Authors authors)
{
    int number_authors = authors_conflicts.size();
    int author_max_conflicts = 0;
    int max_conflicts = 0;

    for (int id_author = 0; id_author < number_authors; id_author++)
    {
        if (authors_conflicts[id_author] > max_conflicts)
        {
            max_conflicts = authors_conflicts[id_author];
            author_max_conflicts = id_author;
        }
    }

    return authors.get_author_articles(author_max_conflicts);
}

int Improvement::get_article_worst_session()
{
    std::vector<int> min_session = {0,0,0};
    int min_session_benefit = 1000000;

    int number_days = benefit_session.size();
    for(int day=0; day<number_days; day++)
    {
        int number_blocks = benefit_session[day].size();
        for(int block=0; block<number_blocks; block++)
        {
            int number_rooms = benefit_session[day][block].size();
            for(int room=0; room<number_rooms; room++)
            {   
                if(benefit_session[day][block][room] < min_session_benefit)
                {
                    min_session_benefit = benefit_session[day][block][room];
                    min_session = {day,block,room};
                }
                
            }
        }
    }

    int number_articles_worst_session = solution_to_improve[min_session[0]][min_session[1]][min_session[2]].size();
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr_articles(0, number_articles_worst_session - 1);
    int id_article_1 = solution_to_improve[min_session[0]][min_session[1]][min_session[2]][distr_articles(gen)];

    return id_article_1;
}

int Improvement::select_article_in_diferent_block(int id_article)
{
    std::vector<int> a_u_1 = article_ubication[id_article];

    std::random_device rd;
    std::mt19937 gen(rd());

    int number_articles = article_ubication.size();
    std::uniform_int_distribution<> distr_articles(0, number_articles - 1);
    int new_article = distr_articles(gen);
    std::vector<int> a_u_2 = article_ubication[new_article];

    //se selecciona otro articulo al azar hasta que se encuentre uno que no este en el mismo bloque que id_article
    while ((a_u_1[0] == a_u_2[0]) && (a_u_1[1] == a_u_2[1]))
    {
        new_article = distr_articles(gen);
        a_u_2 = article_ubication[new_article];
    }

    return new_article;
}

int Improvement::select_article_most_authors_conflicts(Authors authors)
{
    int number_authors = authors_conflicts.size();
    int author_max_conflicts = 0;
    int max_conflicts = 0;

    for (int id_author = 0; id_author < number_authors; id_author++)
    {
        if (authors_conflicts[id_author] > max_conflicts)
        {
            max_conflicts = authors_conflicts[id_author];
            author_max_conflicts = id_author;
        }
    }

    std::vector<int> author_articles = authors.get_author_articles(author_max_conflicts); 
    int number_articles_author = author_articles.size();
    std::vector<int> list_number_conflicts(number_articles_author,0);

    int index_article_max_conflicts = 0;
    int n_max_conflicts = 0;

    for(int i=0; i<number_articles_author; i++)
    {
        int id_article_1 = author_articles[i];
        int day_article_1 = article_ubication[id_article_1][0];
        int block_article_1 = article_ubication[id_article_1][1];
        int sesison_article_1 = article_ubication[id_article_1][2];    
        
        for(int j=i+1; j<number_articles_author;j++)
        {
            int id_article_2 = author_articles[j];
            int day_article_2 = article_ubication[id_article_2][0];
            int block_article_2 = article_ubication[id_article_2][1];
            int sesison_article_2 = article_ubication[id_article_2][2];

            if((day_article_1 == day_article_2) & (block_article_1 == block_article_2))
            {   
                //si las sesiones son diferentes no se cumplen con las restricciones
                if(sesison_article_1 != sesison_article_2)
                {
                    list_number_conflicts[i] += 1;
                    list_number_conflicts[j] += 1;
                    
                    if(list_number_conflicts[i] > n_max_conflicts)
                    {
                        index_article_max_conflicts = i;
                        n_max_conflicts = list_number_conflicts[i];
                    }
                }
            }
        }
    } 

    //std::cout<<"Article most conflicts: "<<author_articles[index_article_max_conflicts]<<std::endl;
    //std::cout<<"Number conflicts:       "<<n_max_conflicts<<std::endl;

    return author_articles[index_article_max_conflicts];
}

int Improvement::select_article_from_most_authors_conflicts(Authors authors, int k)
{
    //se ordenan los autores por cantidad de conflictos
    bool debug = false;
    std::vector<int> index_authors(authors.get_number_authors(), 0);
    
    for (int i = 0 ; i != authors.get_number_authors() ; i++) {
        index_authors[i] = i;
    }

    //se ordenan los autores segun la cantidad de topes de horario
    std::sort(index_authors.begin(), index_authors.end(),
        [&](const int& a, const int& b) {
            return (authors_conflicts[a] > authors_conflicts[b]);
        }
    );

    int max_k_authors = authors.get_number_authors();
    if(k < max_k_authors)
    {
        max_k_authors = k;
    }

    //seleccion del author dentro de los k con mas topes de horario
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr_authors(0, max_k_authors - 1);
    int id_author = index_authors[distr_authors(gen)];

    //seleccion del articulo del autor seleccionado
    int number_articles_author = authors.get_author_articles(id_author).size();
    std::uniform_int_distribution<> distr_articles(0, number_articles_author - 1);
    int index_article = distr_articles(gen);
    int article_id = authors.get_author_articles(id_author)[index_article];

    if(debug)
    {
        for (int i = 0 ; i != max_k_authors ; i++) {
            std::cout << index_authors[i]<<","<<authors_conflicts[index_authors[i]]<< std::endl;
        }

        std::cout << "id_author: "<<id_author<< std::endl;
        std::cout << "id_article_1: "<<article_id<< std::endl;
        std::cout << article_ubication[article_id][0]<<","<<article_ubication[article_id][1]<<std::endl;    
    }

    return article_id;
}

int Improvement::select_article_same_day_diferent_block(int id_article)
{
    int day = article_ubication[id_article][0];
    int number_blocks = solution_to_improve[day].size();
    bool debug = false;
    std::mt19937 gen(rd());

    if(number_blocks>1){
        
        int block = article_ubication[id_article][1];

        std::uniform_int_distribution<> distr_block(0, number_blocks - 1);
        int new_block = distr_block(gen);

        while(new_block == block)
        {
            new_block = distr_block(gen);
        }

        int number_rooms = solution_to_improve[day][new_block].size();
        std::uniform_int_distribution<> distr_rooms(0, number_rooms - 1);
        int new_room = distr_rooms(gen);

        int number_article_session = solution_to_improve[day][new_block][new_room].size();
        std::uniform_int_distribution<> distr_article(0, number_article_session - 1);
        int id_article_2 = solution_to_improve[day][new_block][new_room][distr_article(gen)];
        
        if(debug)
        {
            std::cout << "Case 1:"<< std::endl;
            std::cout << "id_article_2: "<< id_article_2<< std::endl;
            std::cout << article_ubication[id_article_2][0]<<","<<article_ubication[id_article_2][1]<<std::endl;
        }

        return id_article_2;
    }
    else
    {
        int number_days = solution_to_improve.size();
        std::uniform_int_distribution<> distr_day(0, number_days - 1);
        int new_day = distr_day(gen); 
        
        while(new_day == day)
        {
            new_day = distr_day(gen);
        }

        std::uniform_int_distribution<> distr_block(0, solution_to_improve[new_day].size() - 1);
        int new_block = distr_block(gen);

        int number_rooms = solution_to_improve[new_day][new_block].size();
        std::uniform_int_distribution<> distr_rooms(0, number_rooms - 1);
        int new_room = distr_rooms(gen);

        int number_article_session = solution_to_improve[new_day][new_block][new_room].size();
        std::uniform_int_distribution<> distr_article(0, number_article_session - 1);
        int id_article_2 = solution_to_improve[new_day][new_block][new_room][distr_article(gen)];

        if(debug)
        {
            std::cout << "Case 2:"<< std::endl;
            std::cout << "id_article_2: "<< id_article_2<< std::endl;
            std::cout << article_ubication[id_article_2][0]<<","<<article_ubication[id_article_2][1]<<std::endl;
        }

        return id_article_2;
    }
}

int Improvement::article_topics_problem(Topics topics)
{
    //int number_days = number_article_per_day_by_topic.size();
    int number_topics = topics.get_number_topics();

    bool topic_problem_not_found = true;
    int id_topic = 0;
    int day = 0;
    
    while(topic_problem_not_found)
    {
        if(number_article_per_day_by_topic[day][id_topic] > topics.get_max_per_day(id_topic))
        {
            topic_problem_not_found = false;
        }
        else
        {
            if(id_topic == number_topics - 1)
            {
                id_topic = 0;
                day += 1;
            }
            else
            {
                id_topic += 1;
            }
        }
    }
    
    std::vector<int> articles_with_topic = topics.get_articles_by_topic(id_topic);
    for(auto id_article : articles_with_topic)
    {
        int day_article_asignation = article_ubication[id_article][0];
        if(day == day_article_asignation)
        {
            return id_article;
        }
    }

    return articles_with_topic[0];
}

int Improvement::select_article_diferent_day(int id_article_1)
{
    int day = article_ubication[id_article_1][0];
    int number_days = solution_to_improve.size();
    bool debug = false;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distr_day(0, number_days - 1);
    int new_day = distr_day(gen); 
    
    while(new_day == day)
    {
        new_day = distr_day(gen);
    }

    std::uniform_int_distribution<> distr_block(0, solution_to_improve[new_day].size() - 1);
    int new_block = distr_block(gen);

    int number_rooms = solution_to_improve[new_day][new_block].size();
    std::uniform_int_distribution<> distr_rooms(0, number_rooms - 1);
    int new_room = distr_rooms(gen);

    int number_article_session = solution_to_improve[new_day][new_block][new_room].size();
    std::uniform_int_distribution<> distr_article(0, number_article_session - 1);
    int id_article_2 = solution_to_improve[new_day][new_block][new_room][distr_article(gen)];

    if(debug)
    {
        std::cout << "Case 2:"<< std::endl;
        std::cout << "id_article_2: "<< id_article_2<< std::endl;
        std::cout << article_ubication[id_article_2][0]<<","<<article_ubication[id_article_2][1]<<std::endl;
    }

    return id_article_2;
}

std::vector<int> Improvement::article_topics_problem_and_topic(Topics topics)
{
    //int number_days = number_article_per_day_by_topic.size();
    int number_topics = topics.get_number_topics();

    bool topic_problem_not_found = true;
    int id_topic = 0;
    int day = 0;
    
    while(topic_problem_not_found)
    {
        if(number_article_per_day_by_topic[day][id_topic] > topics.get_max_per_day(id_topic))
        {
            topic_problem_not_found = false;
        }
        else
        {
            if(id_topic == number_topics - 1)
            {
                id_topic = 0;
                day += 1;
            }
            else
            {
                id_topic += 1;
            }
        }
    }
    
    std::vector<int> articles_with_topic = topics.get_articles_by_topic(id_topic);
    for(auto id_article : articles_with_topic)
    {
        int day_article_asignation = article_ubication[id_article][0];
        if(day == day_article_asignation)
        {
            return {id_article, id_topic};
        }
    }

    return {articles_with_topic[0],id_topic};
}

int Improvement::select_article_diferent_dayV2(Topics topics,int id_article_1, int id_topic)
{
    int day_problem = article_ubication[id_article_1][0];
    int number_articles = article_ubication.size();
    bool debug = false;
   
    std::mt19937 gen(rd());
    std::vector<int> articles_with_topic = topics.get_articles_by_topic(id_topic);
    
    std::vector<int> articles_without_topic;

    for(int article=0; article<number_articles; article++)
    {
        if(articles_with_topic.size()>0)
        {
            if(article != articles_with_topic[0])
            {
                articles_without_topic.push_back(article);
            }
            else
            {
                articles_with_topic.erase(articles_with_topic.begin());
            }
        }
        else
        {
            articles_without_topic.push_back(article);
        }
    }

    std::uniform_int_distribution<> distr_articles(0, articles_without_topic.size() - 1);
    int id_article_2 = articles_without_topic[distr_articles(gen)]; 

    while(day_problem == article_ubication[id_article_2][0])
    {
        id_article_2 = articles_without_topic[distr_articles(gen)]; 
    }
    
    if(debug)
    {
        std::cout << "id_article_1: "<< id_article_1<< std::endl;
        std::cout << article_ubication[id_article_1][0]<<","<<article_ubication[id_article_1][1]<<std::endl;
        std::cout << "id_article_2: "<< id_article_2<< std::endl;
        std::cout << article_ubication[id_article_2][0]<<","<<article_ubication[id_article_2][1]<<std::endl;
        std::cout << "id_topic problem: "<< id_topic<< std::endl;
        std::vector<int> topics_article_2 = topics.get_article_topics(id_article_2);
        
        std::cout<<"Topicos articulo 2: ";
        for(auto topic:topics_article_2)
        {
            std::cout<<topic<<",";
        }
        std::cout<<std::endl;
        std::vector<int>().swap(topics_article_2);
    }

    std::vector<int>().swap(articles_without_topic);

    return id_article_2;
}