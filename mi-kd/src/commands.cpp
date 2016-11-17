/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <time.h>
#include <ctime>
#include <utility>

#include "../include/commands.h"
#include "../include/node.h"
#include "../include/memory.h"

#define buffer_size 10000 // B

using namespace std;

typedef pair<pair<string, string>, pair<double, double>> data;

/*
    cp -ext

    Copy extern file to the current node.
*/
void commands::_cp_extern_file(memory* part, node* curr, char* extern_file_name)
{
    FILE* file = fopen(extern_file_name, "rb"); // read binary

    char* buffer = new char[buffer_size];
    int vl = fread(buffer, 1, buffer_size, file);
    int crfr;
    int frnum = 0;
    if (vl) // if file is not empty
    {
        curr->start = part->fff_fragment();
        crfr = curr->start;
    }
    while (vl)
    {
        pair<int, int> x = part->add_extern_file(curr, buffer, vl, crfr, frnum);
        crfr = x.first;
        frnum = x.second;
        vl = fread(buffer, 1, buffer_size, file);
    }

    delete[] buffer;

    fclose(file);

    curr->date_modf = time(0); // last time updated
}

/*
    mkdir
*/
node* commands::_make_directory(memory* part, data x)
{
    node* nxt;

    string s = x.first.first;
    char cstr[40];
    strcpy(cstr, s.c_str());
    nxt = part->create_node(cstr);

    s = x.first.second;
    strcpy(cstr, s.c_str());
    _cp_extern_file(part, nxt, cstr);

    nxt->x = x.second.first;
    nxt->y = x.second.second;

    return nxt;
}

/*
    Print file.
*/
void commands::print_file(memory* part, node* curr)
{
    int frsz = fr_mem * 1024 - 2 - 4; // fr_mem - sizeof(short) - sizeof(int);
    int from_fr = curr->start;
    int sz = curr->sizeB;

    //node; optional
    curr->print_node();

    // file
    while (from_fr != 0 && sz)
    {
        for (int i = 0; i < frsz && sz; i++)
        {
            printf("%c", *((char*)(part->mem[from_fr] + 2 + i)) );
            sz--;
        }

        from_fr = *((int*)( (char*)(part->mem[from_fr] + fr_mem * 1024 - 4)) );
    }

    printf("\n");
}
