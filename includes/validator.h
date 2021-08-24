#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include "data.h"
#include "solutions.h"

class Validator{
    std::vector<std::string>  comments;

    public:
        //Verifica que no hayan articulos de un mismo autor en sesiones paralelas
        void articles_in_diferent_sessions(Data, Authors, Solutions);
        
        //Verifica que todos los articulos esten asignados solo una vez
        void article_assignment(Data, Solutions);

        //Verifica que se cumpla la capacidad de articulos por sesion
        void capacity_session(Sessions,Solutions);

        //Verifica la cantidad maxima de articulos por dia que pueden ser asignados que hagan referencia a un mismo topico
        void capacity_topics(Topics,Solutions);

        //Entrega la calidad de la solucion
        void quality_solution(Articles,Solutions);

        std::vector<std::string> get_comments(void);
        void show_comments(void);    
};

#endif