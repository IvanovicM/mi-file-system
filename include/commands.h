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

/*
    This class defines methods for every command in mi-file-system.
*/

class commands
{
    private:
        std::map<std::string, int> mm; // map of commands

    public:
        void map_commands();
        void read_commands();
};

#endif
