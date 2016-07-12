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
#include <sstream>
#include <iostream>

#include "../include/node.h"
#include "../include/memory.h"
#include "../include/commands.h"

node* root;

using namespace std;

int main()
{
    memory* part = new memory();
    part->initialize_mem();
    root = part->create_root();

//    // testing
//    part->test_fff_fragment();
//    part->test_fff_field();

    commands* comm = new commands();
    comm->read_commands(part, root, root);

    return 0;
}

//int main()
//{
//    string s = "/home/marina/ivanovic";
//    string delimiter = "/";
//
//    size_t pos = 0;
//    string token;
//    while ((pos = s.find(delimiter)) != string::npos)
//    {
//        token = s.substr(0, pos);
//        if (token.empty())
//                printf("idi u R\n");
//        else
//            printf("%s\n", token.c_str());
//        s.erase(0, pos + delimiter.length());
//    }
//    printf("%s\n", s.c_str());
//
//    return 0;
//}
