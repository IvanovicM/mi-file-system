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

int main()
{
    // memory initialization
    memory* part = new memory();
    part->initialize_mem();
    commands* cmd = new commands();

    // input
    int n;
    scanf("%d", &n);
    pair<pair<string, string>, pair<double, double>>* g = new pair<pair<string, string>, pair<double, double>>[n];
//    FILE* sfile;
//    sfile = fopen("files.txt", "r");
//    for (int i = 0; i < n; i++)
//    {
//        fscanf(sfile, "%lf %lf", &g[i].second.first, &g[i].second.second);
//        char s[40];
//        fscanf(sfile, "%s", s); cout<<g[i].second.first<<endl;
//        g[i].first.first = s;
//        fscanf(sfile, "%s", s);
//        g[i].first.second = s;
//
//        cout<<g[i].first.first<<" "<<g[i].first.second<<endl;
//    }
//    fclose(sfile);
    for (int i = 0; i < n; i++)
    {
        scanf("%lf %lf", &g[i].second.first, &g[i].second.second);

        char s[40];
        scanf("%s", &s);
        g[i].first.first = s;
        scanf("%s", &s);
        g[i].first.second = s;
    }

    // creating tree
    kdtree* tree = new kdtree(n);
    tree->root = tree->create(part, cmd, g, 0, n-1, true);

    // testing
    test* tst = new test();
    tst->testNNsearch(part, cmd, tree);

    return 0;
}


