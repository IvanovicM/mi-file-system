/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __NODE
#define __NODE

#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <string.h>

/*
    Each object of this class is folder or file in mi-file-system.
*/

class node
{
    public:
        char name[40]; // name of file; max length is 40
        int indx; // its unique ID
        int fragm; // what fragment this node is in
        bool folder; // indicator for folders and files
        int sizeB; // how big this file or folder is
        time_t date_modf; // last date modified
        int start; // int of the fragment that file starts from; 0 for end, in that fragment cannot be file because in that fragment is root

        double x; // x coordinate
        double y; // y coordinate
        node* left; // first son
        node* right; // second son
        node* parent; // its parent
        bool cmpX; // which coordinate node divides space by

        node* itsfile; // its file

    public:
        node();
        node(double x, double y);

        std::string last_date_modf();
        void print_node();
};

#endif
