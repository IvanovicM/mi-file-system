/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#ifndef __MEMORY
#define __MEMORY

#include <stdio.h>
#include <cstdlib>
#include <vector>

#include "../include/node.h"

// constants for fragments (KB)
#define full_mem 1000000
#define fr_mem 4
#define fr_num full_mem/fr_mem

/*
    Methods from this class allocate and deallocate memory.
*/

class memory
{
    public:
        memory();

        std::vector<int> free_fr;

        void* mem[fr_num]; // memory that file-system can use
        int node_num; // current node unique ID
        int max_nodes_in_fragment; // max number of nodes in fragments
        node* root; // pointer to the root in the tree
        node* curr; // pointer to the current node in the tree

        void initialize_mem();
        std::pair<int, int> fff_field();
        int fff_fragment();
        node* create_node(char* name);
        std::pair<int, int> add_extern_file(node* curr, char* buffer, int vl, int crfr, int frnum);
};

#endif
