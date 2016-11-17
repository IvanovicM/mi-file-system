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
#include <iostream>

#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"
#include "../include/kdtree.h"
#include "../include/test.h"

using namespace std;

typedef pair<pair<string, string>, pair<double, double>> data;

int main()
{
    // memory initialization
    memory* part = new memory();
    part->initialize_mem();
    commands* cmd = new commands();

    // input
    int n;
    char buff[40];
    FILE* sfile;
    sfile = fopen("files.txt", "r");
    fscanf(sfile, "%d", &n);
    data* g = new data[n];
    for (int i = 0; i < n; i++)
    {
        fscanf(sfile, "%lf %lf", &g[i].second.first, &g[i].second.second);
        fscanf(sfile, "%s", buff);
        g[i].first.first = buff;
        fscanf(sfile, "%s", buff);
        g[i].first.second = buff;
    }
    fclose(sfile);

    // creating tree
    kdtree* tree = new kdtree(n);
    tree->root = tree->create(part, cmd, g, 0, n-1, true);

    // testing
    test* tst = new test();
    tst->testNNsearch(part, cmd, tree);

    return 0;
}


