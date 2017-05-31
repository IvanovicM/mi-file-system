/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <utility>
#include <math.h>

#include "../include/test.h"
#include "../include/node.h"
#include "../include/kdtree.h"
#include "../include/memory.h"
#include "../include/commands.h"

using namespace std;

void test::testNNsearch(memory* part, commands* cmd, kdtree* tree)
{
    int q;
    scanf("%d", &q);

    while (q--)
    {
        double x, y;
        scanf("%lf %lf", &x, &y);
        node* best = tree->NNsearch(x, y);
        cmd->print_file(part, best);
        //printf("nearest: %s\n", best->name);
    }
}
