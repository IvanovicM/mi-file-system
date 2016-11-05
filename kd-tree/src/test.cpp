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

#define PARENT(i) ((i-1) / 2)
#define LINE_WIDTH 70

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

    printf("\nnode: (%f, %f)\n", nd->x, nd->y);
    if (nd->left != NULL)
        printf("left: (%f, %f)\n", nd->left->x, nd->left->y);
    if (nd->right != NULL)
        printf("right: (%f, %f)\n", nd->right->x, nd->right->y);

    testCreate(nd->left);
    testCreate(nd->right);
}
