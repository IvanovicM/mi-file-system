
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

class test
{
    public:
        void testQuickselect(int n, std::pair<double, double>* g, kdtree* tree);
        void testCreate(kdtree* tree);

};

#endif
