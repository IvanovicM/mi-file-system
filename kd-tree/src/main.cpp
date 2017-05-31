/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <utility>

#include "../include/test.h"
#include "../include/kdtree.h"
#include "../include/node.h"

using namespace std;

int main()
{
    int n;

    // input
    scanf("%d", &n);
    pair<double, double>* g = new pair<double, double>[n];
    for (int i = 0; i < n; i++)
        scanf("%lf %lf", &g[i].first, &g[i].second);

    // creating tree
    kdtree* tree = new kdtree(n);
    tree->root = tree->create(g, 0, n-1, true);

//    // testing
    test* tst = new test();
//    tst->testQuickselect(n, g, tree);
//    tst->testCreate(tree->root);
    tst->testNNsearch(tree);

    return 0;
}
