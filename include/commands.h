/*
    Marina Ivanovic
    Project in Petnica Science Center
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

/*
    This class defines methods for every command in mi-file-system.
*/

class commands
{
    private:
        std::map<std::string, int> mm; // codes of commands

    public:
        void input_error();

        void _list_directory(node* curr);
        node* _change_directory(node* curr);
        void _make_directory(memory* part, node* curr, std::string sub);
        void _make_file(memory* part, node* curr, std::string sub);
        void _delete(memory* part, node* root, node* curr, node* del);
        void _cp_extern_file(memory* part, node* curr, char* extern_file_name);
        void _cp_intern_file(memory* part, node* from, node* to);

        void __help(std::string cmd);

        void print_path(node* curr, node* root);
        void print_file(memory* part, node* curr);
        node* find_son(node* parent, char* son_name);
        void destination(memory* part, node* root, node* start_from, std::string dest);
        void map_commands();
        void read_commands(memory* part, node* curr, node* root);
};

#endif
