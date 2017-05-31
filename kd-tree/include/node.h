
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
        double x; // x coordinate
        double y; // y coordinate
        node* left; // first son
        node* right; // second son
        node* parent; // its parent
        bool cmpX; // which coordinate node divides space by

    public:
        node();
        node(double x, double y);
};

#endif
