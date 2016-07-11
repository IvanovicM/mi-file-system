/*
    Marina Ivanovic
    Project in Petnica Science Center
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
        int sizeB; // how gig this file or folder is
        time_t date_modf; // last date modified
        void* start; // pointer to the beginning of a file if it is not a folder
        node* parent; // its parent

        node* older_from; // node that is parent or is connected to parent and tells who is parent
        node* older_to; // node whose older_from is this node
        node* younger; // first son

    public:
        std::string last_date_modf();
        void print_node();
};

#endif
