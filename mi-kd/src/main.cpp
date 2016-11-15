/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <limits.h>
#include <sstream>
#include <iostream>
#include <utility>

#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"
#include "../include/kdtree.h"
#include "../include/test.h"

using namespace std;

int main()
{
    // memory initialization
    memory* part = new memory();
    part->initialize_mem();

    // input
    int n;
    scanf("%d", &n);
    pair<double, double>* g = new pair<double, double>[n];
    for (int i = 0; i < n; i++)
        scanf("%lf %lf", &g[i].first, &g[i].second);

    // creating tree
    kdtree* tree = new kdtree(n);
    tree->root = tree->create(g, 0, n-1, true);

    // testing
    test* tst = new test();
    tst->testNNsearch(tree);

//    commands* comm = new commands();
//    comm->read_commands(part, root, root);

    return 0;
}


