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

void test::testQuickselect(int n, pair<double, double>* g, kdtree* tree)
{
    int k = 3, l = 0, r = 7;
    pair<double, double> x = tree->quickselect(g, l, r, k + l, true);
    printf("%d-th element in the interval [%d, %d] of array is: (%f %f).\n", k, l, r, x.first, x.second);
}

void test::testCreate(node* nd)
{
    if (nd == NULL) return;

    if (nd->parent != NULL)
        printf("node: (%f, %f), parent: (%f, %f)\n",  nd->x, nd->y, nd->parent->x, nd->parent->y);

//    printf("\nnode: (%f, %f)\n", nd->x, nd->y);
//    if (nd->left != NULL)
//        printf("left: (%f, %f)\n", nd->left->x, nd->left->y);
//    if (nd->right != NULL)
//        printf("right: (%f, %f)\n", nd->right->x, nd->right->y);

    testCreate(nd->left);
    testCreate(nd->right);
}

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
