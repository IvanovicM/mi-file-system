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

#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"

node* root;
node* curr;

using namespace std;

int main()
{
    memory* part = new memory();
    part->initialize_mem();
    root = part->create_root();
    curr = root;

//    // testing
//    part->test_fff_fragment();
//    part->test_fff_field();

    commands* comm = new commands();
    comm->read_commands();

    return 0;
}
