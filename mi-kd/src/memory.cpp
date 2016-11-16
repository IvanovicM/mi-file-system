/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <vector>

#include "../include/node.h"
#include "../include/memory.h"

// constants for fragments (KB)
#define full_mem 1000000
#define fr_mem 4
#define fr_num full_mem/fr_mem

using namespace std;

memory::memory()
{
    this->node_num = 0;
    this->max_nodes_in_fragment = (fr_mem * 1024 - 2) / (sizeof(node) + 1);
}

/*
    Memory initialization

    Divides memory in fr_num fragments, each with fr_mem KB.
*/
void memory::initialize_mem()
{
    for (int i = fr_num - 1; i >= 0; i--)
    {
        mem[i] = malloc(fr_mem * 1024);

        // free fragment
        for(int j = 0; j < fr_mem * 1024; j++)
            *((char*)(mem[i] + j)) = 0;

        free_fr.push_back(i);
    }
}

/*
    Find First Free Field

    Input: /

    Output: pair of 2 integers (i, j) that can made node* as mem[i] + 3 + j * (sizeof(node) + 1))
            i is index of fragment
            j is index of field in that fragment
*/
pair<int, int> memory::fff_field()
{
    // finding fragment with some nodes already, but is not filled
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) < max_nodes_in_fragment && *((short*)(mem[i])) > 0 )
        {
            *((short*)(mem[i])) = *((short*)(mem[i])) + 1; // in this field is one node added
            for (int j = 0; j < max_nodes_in_fragment; j++)
            {
                if ( *((char*)(mem[i] + 2 + j * (sizeof(node) + 1))) == 0 )
                {
                    *((char*)(mem[i] + 2 + j * (sizeof(node) + 1))) = 1; // filled field with some node
                    return make_pair(i, j);
                }
            }
        }
    }

    int i = free_fr.back();
    free_fr.pop_back();
    *((short*)(mem[i])) = *((short*)(mem[i])) + 1; // in this field is one node added
    *((char*)(mem[i] + 2)) = 1; // filled field with some node
    return make_pair(i, 0);
}

/*
    Find First Free Fragment

    Input: /

    Output: 1 integers which is index of free fragment
*/
int memory::fff_fragment()
{
    int i = free_fr.back();
    free_fr.pop_back();
    return i;
}

/*
    Function for creating node

    Input: name of node, indicator if it is folder or file, pointer to its parent, pointer to its parent or node that has same parent and pointer to node that should have the same parent but is not connected with it

    Output: pointer to node that is created
*/
node* memory::create_node(char* name)
{
    // finding first free field of free memory
    pair<int, int> x = fff_field();
    node* beg = (node*)(mem[x.first] + 3 + x.second * (sizeof(node) + 1));

    // given parameters
    strcpy(beg->name, name);
    beg->parent = NULL;

    beg->indx = node_num;
    node_num++;

    // when created
    beg->start = 0;
    beg->fragm = x.first;
    beg->sizeB = 0;
    beg->date_modf = time(0);
    beg->start = fff_fragment();

    return beg;
}

/*
    Adding data to file from extern file.
*/
pair<int, int> memory::add_extern_file(node* curr, char* buffer, int vl, int crfr, int frnum)
{
    int sz = curr->sizeB;
    int frsz = fr_mem * 1024 - 2 - 4; // fr_mem - sizeof(short)[indicator at the beginning] - sizeof(int)[pointer to the continuation of the file];

    // putting data to this file
    int crsz = sz - frnum * frsz;
    for (int i = 0; i < vl; i++)
    {
        if (crsz == frsz)
        {
            crsz = 0; // reset
            *((int*)( (char*)(mem[crfr] + fr_mem * 1024 - 4)) ) = fff_fragment();
            crfr = *((int*)( (char*)(mem[crfr] + fr_mem * 1024 - 4)) );
            frnum++;

//            if (crfr == -1)
//            {
//                // optional
//                // if file system is full
//            }
        }

        *((char*)(mem[crfr] + 2 + crsz)) = buffer[i];
        crsz++;
    }

    curr->sizeB += vl;

    return make_pair(crfr, frnum);
}
