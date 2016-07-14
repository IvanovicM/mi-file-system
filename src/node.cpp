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
