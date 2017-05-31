/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <cstdlib>
#include <time.h>

#include "../include/node.h"
#include "../include/memory.h"

using namespace std;

/*
    Constructor
*/
node::node(double x, double y)
{
    this->x = x;
    this->y = y;
}

/*
    Get current date/time, format is YYYY-MM-DD.HH:mm:ss
*/
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
    printf("%-23s %-7d %s\n", last_date_modf().c_str(), sizeB, name);
    printf("\nx: %f; y: %f\n\n", x, y);
}
