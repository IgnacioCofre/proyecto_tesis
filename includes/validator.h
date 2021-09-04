#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <math.h>
#include "data.h"
#include "solutions.h"

class Validator{
    std::vector<std::string>  comments;

    public:
        //Verifica que no hayan articulos de un mismo autor en sesiones paralelas
        int articles_in_diferent_sessions(Data, Authors, std::vector<std::vector<std::vector<std::vector<int>>>>);
        
        //Verifica que todos los articulos esten asignados solo una vez
        void article_assignment(Data, Solutions);

        //Verifica que se cumpla la capacidad de articulos por sesion
        void capacity_session(Sessions,std::vector<std::vector<std::vector<std::vector<int>>>>);

        /*
        Verifica la cantidad maxima de articulos por dia que pueden ser asignados que hagan 
        referencia a un mismo topico
        Entrega la cantidad de articulos que se pasan del maximo permitido por dia para todos
        los topicos diponibles
        */
        int capacity_topics(Topics,std::vector<std::vector<std::vector<std::vector<int>>>>);

        //Entrega la calidad de la solucion
        int solution_benefit(Articles,std::vector<std::vector<std::vector<std::vector<int>>>>);

        std::vector<std::string> get_comments(void);
        void show_comments(void);    
};

#endif