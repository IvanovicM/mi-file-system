/*
    Marina Ivanovic
    Project in Petnica Science Center
*/

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <limits.h>
#include <sstream>
#include <iostream>

#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"

using namespace std;

int main()
{
    memory* part = new memory();
    part->initialize_mem();
    node* root = part->create_root();

    commands* comm = new commands();
    comm->read_commands(part, root, root);

    return 0;
}


