/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <algorithm>

#include "../include/kdtree.h"

using namespace std;

/*
    Constructor.
*/
kdtree::kdtree (int n)
{
    this->n = n;
    this->g = new int[n];
}

/*
    Finds k-th element in the array 'g'.
*/
int kdtree::quickselect(int l, int r, int k)
{
    // pivot = a[r]
    int smallIndx = l - 1; // last smaller than pivot

    // smaller elements are in interval [l, smallIndx]
    // larger or equal elements are in interval [smallIndx+1, r]
    for (int i = l; i < r; i++)
    {
        if (g[i] < g[r])
        {
            smallIndx++;
            swap(g[i], g[smallIndx]);
        }
    }

    // pitting pivot on the its position in the array
    int p = smallIndx + 1;
    swap(g[p], g[r]);

    // checking where in the array pivot is
    if (p == k)
    {
        return g[p];
    }
    else if (p > k)
    {
        return quickselect(l, p - 1, k);
    }
    else
    {
        return quickselect(p + 1, r, k);
    }
}
