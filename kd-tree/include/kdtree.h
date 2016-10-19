/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __NODE
#define __NODE

#include <stdio.h>

class kdtree
{
    public:
        int n; // Number of nodes.
        int* g; // Array of nodes.

    public:
        kdtree(int n);

        int quickselect(int l, int r, int k);

};

#endif
