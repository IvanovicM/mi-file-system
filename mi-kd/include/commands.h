/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __COMMANDS
#define __COMMANDS

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>

#include "../include/node.h"
#include "../include/memory.h"

typedef std::pair<std::pair<std::string, std::string>, std::pair<double, double>> data;

/*
    This class defines methods for every command in mi-file-system.
*/

class commands
{
    public:
        node* _make_directory(memory* part, data x);
        void _cp_extern_file(memory* part, node* curr, char* extern_file_name);
        void print_file(memory* part, node* curr);
};

#endif
