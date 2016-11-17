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

typedef pair<pair<string, string>, pair<double, double>> data;

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
data kdtree::quickselect(data* g, int l, int r, int k, bool cmpX)
{
    // pivot = g[r]
    int smallIndx = l - 1; // last smaller than pivot

    // smaller elements are in interval [l, smallIndx]
    // larger or equal elements are in interval [smallIndx+1, r]
    if (cmpX) // compare to the first coordinate (X)
    {
        for (int i = l; i < r; i++)
        {
            if (g[i].second.first < g[r].second.first)
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
            if (g[i].second.second < g[r].second.second)
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
node* kdtree::create(memory* part, commands* cmd, data* g, int l, int r, bool cmpX)
{
    // subtree that does not exist
    if (r<l) return NULL;

    // making root of subtree and its sons
    int k = (r-l+1)/2 + l;
    data x = quickselect(g, l, r, k, cmpX);
    node* nd = cmd->_make_directory(part, x);
    nd->left = create(part, cmd, g, l, k-1, !cmpX);
    nd->right = create(part, cmd, g, k+1, r, !cmpX);

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

    // go to left and right
    if (curr->parent == from)
    {
        DFS(best, curr->left, curr, Xx, Xy);
        DFS(best, curr->right, curr, Xx, Xy);
    }
    else
    {
        double curr_dist = dist(*best, Xx, Xy);
        if (curr->cmpX) // parent compares by X coordinate
        {
            if (curr->left != from && Xx >= curr->x && curr_dist > abs(Xx - curr->x))
                DFS(best, curr->left, curr, Xx, Xy);

            if (curr->right != from && Xx <= curr->x && curr_dist > abs(Xx - curr->x))
                DFS(best, curr->right, curr, Xx, Xy);
        }
        else // parent compares by Y coordinate
        {
            if (curr->left != from && Xy >= curr->y && curr_dist > abs(Xy - curr->y))
                DFS(best, curr->left, curr, Xx, Xy);

            if (curr->right != from && Xy <= curr->y && curr_dist > abs(Xy - curr->y))
                DFS(best, curr->right, curr, Xx, Xy);
        }
    }

    // go to parent if it was not visited
    if (curr->parent != from)
        DFS(best, curr->parent, curr, Xx, Xy);
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
