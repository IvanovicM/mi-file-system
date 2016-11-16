
/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __TEST
#define __TEST

#include <stdio.h>
#include <utility>

#include "../include/kdtree.h"
#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"

class test
{
    public:
        void testNNsearch(memory* part, commands* cmd, kdtree* tree);
};

#endif
