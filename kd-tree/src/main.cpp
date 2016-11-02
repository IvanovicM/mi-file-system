/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <utility>

#include "../include/kdtree.h"
#include "../include/node.h"

using namespace std;

void testQS(int n, pair<double, double>* g, kdtree* tree)
{
    int k = 3, l = 2, r = 6;
    pair<double, double> x = tree->quickselect(g, l, r, k + l, true);
    printf("%d-th element in the interval [%d, %d] of array is: (%f %f).\n", k, l, r, x.first, x.second);
}

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
    node* root;
    root = tree->create(g, 0, n-1, true);
    //testQS(n, g, tree);

//    for (int i = 0; i < n; i++)
//        printf("%f, %f\n", g[i].first, g[i].second);
    return 0;
}
