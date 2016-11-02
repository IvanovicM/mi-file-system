
/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __NODE
#define __NODE

#include <stdio.h>

class node
{
    public:
        double x, y; // x, y coordinates
        node* fstSon; // first son
        node* secSon; // second son

    public:
        node();
        node(double x, double y);
};

#endif
