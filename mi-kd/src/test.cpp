/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <utility>
#include <math.h>

#include "../include/test.h"
#include "../include/node.h"
#include "../include/kdtree.h"

using namespace std;

void test::testNNsearch(kdtree* tree)
{
    int q;
    scanf("%d", &q);

    while (q--)
    {
        double x, y;
        scanf("%lf %lf", &x, &y);
        node* best = tree->NNsearch(x, y);
        printf("nearest: %f %f\n", best->x, best->y);
    }
}
