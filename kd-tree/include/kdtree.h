/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __KDTREE
#define __KDTREE

#include <stdio.h>
#include <utility>

class kdtree
{
    public:
        int n; // Number of nodes.

    public:
        kdtree(int n);

        std::pair<double, double> quickselect(std::pair<double, double>* g, int l, int r, int k, bool cmpX);
        void create(std::pair<double, double>* g);

};

#endif
