#include <stdio.h>
#include <cstdlib>

class node
{
    public:
        char name[40]; // name of file; max length is 40
        int indx; // its unique ID
        int fragm; // what fragment this node is in
        bool folder; // indicator for folders and files
        void* start; // pointer to the beginning of a file if it is not a folder
        node* parent; // its parent

        node* older_from; // node that is parent or is connected to parent and tells who is parent
        node* older_to; // node whose older_from is this node
        node* younger; // first son
};

