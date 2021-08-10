#include <iostream>
#include <fstream>
#include <string>
#include "includes/function.h"
#include "includes/class.h"

int main() {
    
    //int x = add(2, 4);
    //std::cout << x << std::endl;

    const char * input_file = "input/ebl_one_author/ebl_one_author_30_1.txt";

    Data data;
    data.read_input_file(input_file);
    data.show_data();
    
    return 0;
    
}