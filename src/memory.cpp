/*
    Marina Ivanovic
    Project in Petnica Science Center
*/

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "../include/node.h"
#include "../include/memory.h"

// constants for fragments (KB)
#define full_mem 2000
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
    for (int i = 0; i < fr_num; i++)
    {
        mem[i] = malloc(fr_mem * 1024);

        // free fragment
        for(int j = 0; j < fr_mem * 1024; j++)
            *((char*)(mem[i] + j)) = 0;
    }

//    // test
//    for (int i = 0; i < 200; i++)
//    {
//        printf("%d\n", *((char*)(mem[8] + i)));
//    }
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

    // finding free fragment if necessary
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) == 0 )
        {
            *((short*)(mem[i])) = *((short*)(mem[i])) + 1; // in this field is one node added
            *((char*)(mem[i] + 2)) = 1; // filled field with some node
            return make_pair(i, 0);
        }
    }
}

/*
    Find First Free Fragment

    Input: /

    Output: 1 integers which is index of free fragment
*/
int memory::fff_fragment()
{
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) == 0 )
        {
            *((short*)(mem[i])) = SHRT_MAX;
            *((int*)( (char*)(mem[i] + fr_mem * 1024 - 4)) ) = 0;
            return i;
        }
    }

    return -1;
}

/*
    Function for creating node

    Input: name of node, indicator if it is folder or file, pointer to its parent, pointer to its parent or node that has same parent and pointer to node that should have the same parent but is not connected with it

    Output: pointer to node that is created
*/
node* memory::create_node(char* name, bool folder, node* parent, node* older_from, node* older_to)
{
    // finding first free field of free memory
    pair<int, int> x = fff_field();
    node* beg = (node*)(mem[x.first] + 3 + x.second * (sizeof(node) + 1));

    // given parameters
    strcpy(beg->name, name);
    beg->folder = folder;
    beg->parent = parent;
    beg->older_from = older_from;
    beg->older_to = older_to;

    if (beg->older_to != NULL)
        (beg->older_to)->older_from = beg;

    beg->indx = node_num;
    node_num++;

    // when created
    beg->younger = NULL;
    beg->start = 0;
    beg->fragm = x.first;
    beg->sizeB = 0;
    beg->date_modf = time(0);

    return beg;
}

/*
    Function for creating folder that is root and cannot be deleted.
*/
node* memory::create_root()
{
    *((short*)(mem[0])) = *((short*)(mem[0])) + 1; // in this field is one node added
    *((char*)(mem[0] + 2)) = 1; // filled field with some node

    pair<int, int> x = make_pair(0, 0);
    node* beg = (node*)(mem[x.first] + 3 + x.second * (sizeof(node) + 1));

    // default parameters for root
    strcpy(beg->name, "R");
    beg->folder = true;
    beg->parent = beg;
    beg->older_to = NULL;

    beg->indx = node_num;
    node_num++;

    // when created
    beg->younger = NULL;
    beg->start = 0;
    beg->fragm = x.first;
    beg->sizeB = 0;
    beg->date_modf = time(0);

    return beg;
}

/*
    Adding data to file from extern file.
*/
void memory::add_extern_file(node* curr, char* buffer, int vl)
{
    int sz = curr->sizeB;
    int frnum = 0;
    int frsz = fr_mem * 1024 - 2 - 4; // fr_mem - sizeof(short)[indicator at the beginning] - sizeof(int)[pointer to the continuation of the file];

    // find fragment for new data
    int crfr = curr->start;
    while (*((int*)( (char*)(mem[crfr] + fr_mem * 1024 - 4)) ) != 0)
    {
        crfr = *((int*)( (char*)(mem[crfr] + fr_mem * 1024 - 4)) );
        frnum++;
    }

    // putting data to this file
    int crsz = sz - frnum * frsz;
    int j = 0;
    for (int i = 0; i < vl; i++)
    {
        if (crsz == frsz)
        {
            crsz = 0; // reset
            j = 0;
            *((int*)( (char*)(mem[crfr] + fr_mem * 1024 - 4)) ) = fff_fragment();
            crfr = *((int*)( (char*)(mem[crfr] + fr_mem * 1024 - 4)) );

//            if (crfr == -1)
//            {
//                // optional
//                // if file system is full
//            }
        }

        crsz++;
        *((char*)(mem[crfr] + 2 + crsz + j)) = buffer[i];

        j++;
    }

    curr->sizeB += vl;
}

/*
    Adding data to file from this file system.
*/
void memory::add_intern_file(node* from, node* curr)
{
    if (from == NULL)
    {
        delete_node(curr);
    }

    int from_fr = from->start;
    int frsz = fr_mem * 1024 - 2 - 4; // fr_mem - sizeof(short) - sizeof(int);

    // start
    int curr_fr;
    if (from_fr != 0)
    {
        curr->start = curr_fr = fff_fragment();

        for (int i = 0; i < frsz; i++)
        {
            *((char*)(mem[curr_fr] + 2 + i)) = *((char*)(mem[from_fr] + 2 + i));
        }

        from_fr = *((int*)( (char*)(mem[from_fr] + fr_mem * 1024 - 4)) );
    }

    // continue
    while (from_fr != 0)
    {
        curr_fr = *((int*)( (char*)(mem[curr_fr] + fr_mem * 1024 - 4)) ) = fff_fragment();

        for (int i = 0; i < frsz; i++)
        {
            *((char*)(mem[curr_fr] + 2 + i)) = *((char*)(mem[from_fr] + 2 + i));
        }

        from_fr = *((int*)( (char*)(mem[from_fr] + fr_mem * 1024 - 4)) );
    }

    curr->sizeB = from->sizeB;
}

/*
    Function for deleting file

    Input; pointer to node which is file that should be deleted

    Output: /
*/
void memory::delete_file(node* del)
{
    while (del->start != 0)
    {
        *((short*)(mem[del->start])) = 0; // empty fragment
        del->start = *((int*)( (char*)(mem[del->start] + fr_mem * 1024 - 4)) );
    }

    del->sizeB = 0;
}

/*
    Function for deleting node

    Input: pointer to node that should be deleted

    Output: /
*/
void memory::delete_node(node* del)
{
    // deleting file if node is a file, but not a folder
    delete_file(del);

    // changing pointers
    if (del->parent == del->older_from) // parent
    {
        (del->older_from)->younger = del->older_to;
    }
    else
    {
        (del->older_from)->older_to = del->older_to;
    }
    if (del->older_to != NULL)
    {
        (del->older_to)->older_from = del->older_from;
    }

    // declaring node as non-existent
    *((short*)(mem[ del->fragm ])) = *((short*)(mem[ del->fragm ])) - 1; // in this fragment is one node less
    *((char*)(  (char*)del - 1)) = 0; // is not filled with node anymore
}
