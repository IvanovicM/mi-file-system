/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <utility>
#include <algorithm>
#include <math.h>

#include "../include/kdtree.h"
#include "../include/node.h"

using namespace std;

/*
    Constructor.
*/
kdtree::kdtree(int n)
{
    this->n = n;
    root = NULL;
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

    // putting pivot on the its position in the array
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
    if (r<l) return NULL;

    // making root of subtree and its sons
    int k = (r-l+1)/2 + l;
    pair<double, double> x = quickselect(g, l, r, k, cmpX);
    node* nd = new node(x.first, x.second);
    nd->left = create(g, l, k-1, !cmpX);
    nd->right = create(g, k+1, r, !cmpX);

    // parent
    if (nd->left)
        nd->left->parent = nd;
    if (nd->right)
        nd->right->parent = nd;

    // dividing space
    nd->cmpX = cmpX;

    return nd;
}

/*
    Distance between a point and a node.
*/
double kdtree::dist(node* a, double x, double y)
{
    return sqrt((a->x - x)*(a->x - x) + (a->y - y)*(a->y - y));
}

/*
    Depth First Search
    // min_dist = min distance between point X and the best neighbor
*/
void kdtree::DFS(node** best, node* curr, node* from, double Xx, double Xy)
{
    if (curr == NULL)
        return;

    // if curr is nearest than best
    if (dist(curr, Xx, Xy) < dist(*best, Xx, Xy))
        *best = curr;

    // go to left
    if (curr->left != from)
        DFS(best, curr->left, curr, Xx, Xy);

    // go to right
    if (curr->right != from)
        DFS(best, curr->right, curr, Xx, Xy);

    // go to parent if it was not visited
    if (curr->parent != NULL && curr->parent != from)
    {
        // go to parent if it is a root of a subtree that should be visited
        if (curr->parent->cmpX) // parent compares by X coordinate
        {
            if (dist(*best, Xx, Xy) > abs(Xx - curr->parent->x))
                DFS(best, curr->parent, curr, Xx, Xy);
        }
        else // parent compares by Y coordinate
        {
            if (dist(*best, Xx, Xy) > abs(Xy - curr->parent->y))
                DFS(best, curr->parent, curr, Xx, Xy);
        }
    }
}

/*
    Nearest neighbor search for given coordinates.
*/
node* kdtree::NNsearch(double x, double y)
{
    // finding the leaf of the tree
    node* best = root;
    bool found = false;
    while (!found)
    {
        if (best->cmpX) // X
        {
            if (x <= best->x) //left
            {
                if (best->left) // exist
                {
                    best = best->left;
                }
                else // does not exist
                {
                    found = true;
                }
            }
            else // right
            {
                if (best->right) // exist
                {
                    best = best->right;
                }
                else // does not exist
                {
                    found = true;
                }
            }
        }
        else // Y
        {
            if (y <= best->y) //left
            {
                if (best->left) // exist
                {
                    best = best->left;
                }
                else // does not exist
                {
                    found = true;
                }
            }
            else // right
            {
                if (best->right) // exist
                {
                    best = best->right;
                }
                else // does not exist
                {
                    found = true;
                }
            }
        }
    }

    // finding the nearest neighbour
    DFS(&best, best, NULL, x, y);

    return best;
}
