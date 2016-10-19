/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>

#include "../include/kdtree.h"

using namespace std;

void testQS(int n, kdtree* tree)
{
    int k = 0, l = 6, r = 6;
    printf("%d-th element in the interval [%d, %d] of array is: %d.\n", k, l, r, tree->quickselect(l, r, k + l));
}

int main()
{
    int n;

    // input
    scanf("%d", &n);
    kdtree* tree = new kdtree(n);
    for (int i = 0; i < n; i++)
        scanf("%d", &tree->g[i]);

    testQS(n, tree);

//    for (int i = 0; i < n; i++)
//        printf("%d, ", tree->g[i]);
    return 0;
}
