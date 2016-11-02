/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <utility>
#include <algorithm>

#include "../include/kdtree.h"
#include "../include/node.h"

using namespace std;

/*
    Constructor.
*/
kdtree::kdtree(int n)
{
    this->n = n;
}

/*
    Finds k-th element in the given interval of array 'g'.
*/
pair<double, double> kdtree::quickselect(pair<double, double>* g, int l, int r, int k, bool cmpX)
{
    // pivot = g[r]
    int smallIndx = l - 1; // last smaller than pivot

    // smaller elements are in interval [l, smallIndx]
    // larger or equal elements are in interval [smallIndx+1, r]
    if (cmpX) // compare to the first coordinate (X)
    {
        for (int i = l; i < r; i++)
        {
            if (g[i].first < g[r].first)
            {
                smallIndx++;
                swap(g[i], g[smallIndx]);
            }
        }
    }
    else // compare to the second coordinate (Y)
    {
        for (int i = l; i < r; i++)
        {
            if (g[i].second < g[r].second)
            {
                smallIndx++;
                swap(g[i], g[smallIndx]);
            }
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
        return quickselect(g, l, p - 1, k, cmpX);
    }
    else
    {
        return quickselect(g, p + 1, r, k, cmpX);
    }
}

/*
    Creates kd-tree with given array of nodes.
*/
node* kdtree::create(pair<double, double>* g, int l, int r, bool cmpX)
{
    // subtree that does not exist
    if (r>l) return NULL;

    // making root of subtree and its sons
    int k = (r-l+1)/2 + l;
    pair<double, double> x = quickselect(g, l, r, k, cmpX);
    node* nd = new node(x.first, x.second);
    nd->fstSon = create(g, l, k-1, !cmpX);
    nd->secSon = create(g, k+1, r, !cmpX);

    return nd;
}
