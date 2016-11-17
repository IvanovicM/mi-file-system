/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __KDTREE
#define __KDTREE

#include <stdio.h>
#include <utility>

#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"

typedef std::pair<std::pair<std::string, std::string>, std::pair<double, double>> data;

class kdtree
{
    public:
        int n; // Number of nodes.
        node* root; // Root of the tree

    public:
        kdtree(int n);

        data quickselect(data* g, int l, int r, int k, bool cmpX);
        node* create(memory* part, commands* cmd, data* g, int l, int r, bool cmpX);
        double dist(node* a, double x, double y);
        void DFS(node** best, node* curr, node* from, double Xx, double Xy);
        node* NNsearch(double x, double y);
};

#endif
