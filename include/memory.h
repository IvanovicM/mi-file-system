/*
    Marina Ivanovic
    Project in Petnica Science Center
*/

#ifndef __MEMORY
#define __MEMORY

#include <stdio.h>
#include <cstdlib>

#include "../include/node.h"

// constants for fragments (KB)
#define full_mem 2000
#define fr_mem 4
#define fr_num full_mem/fr_mem

/*
    Methods from this class allocate and deallocate memory.
*/

class memory
{
    public:
        memory();

        void* mem[fr_num]; // memory that file-system can use
        int node_num; // current node unique ID
        int max_nodes_in_fragment; // max number of nodes in fragments
        node* root; // pointer to the root in tree
        node* curr; // pointer to the current node in tree

        void initialize_mem();
        std::pair<int, int> fff_field();
        int fff_fragment();
        node* create_node(char* name, bool folder, node* parent, node* older_from, node* older_to);
        node* create_root();
        void delete_node(node* del);
        void delete_file(node* del);
};

#endif
