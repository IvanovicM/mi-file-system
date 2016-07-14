/*
    Marina Ivanovic
    Project in Petnica Science Center
*/

#include <stdio.h>
#include <cstdlib>
#include <time.h>

#include "../include/node.h"
#include "../include/memory.h"

using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string node::last_date_modf()
{
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&date_modf);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

void node::print_node()
{
    printf("%-23s %-10s %-7d %s\n", last_date_modf().c_str(), folder ? "<DIR>" : "", sizeB, name);
}

void node::print_file(memory* part);
{
    if (folder)
    {
        printf("This is not a file.\n");
        return;
    }

    int frsz = fr_mem * 1024 - 2 - 4; // fr_mem - sizeof(short) - sizeof(int);
    int from_fr = start;

    //node
    print_node();

    // file
    while (from_fr != 0)
    {
        for (int i = 0; i < frsz; i++)
        {
            printf("%c", *((char*)(part->mem[from_fr] + 2 + i)) );
        }

        from_fr = *((int*)( (char*)(part->mem[from_fr] + fr_mem * 1024 - 4)) );
    }

    printf("\n");
}
