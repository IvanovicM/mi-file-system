#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <limits.h>

#include "../include/node.h"

// constants for fragments (KB)
#define full_mem 2000
#define fr_mem 4
#define fr_num full_mem/fr_mem

using namespace std;

void* mem[fr_num];
int node_num = 0;
int max_nodes_in_fragment;
node* root;
map<string, int> mm;

/*
    Memory initialization

    Divides memory in fr_num fragmentf, each with fr_mem KB.
*/
void initialize_mem()
{
    for (int i = 0; i < fr_num; i++)
    {
        mem[i] = malloc(fr_mem * 1024);

        // free fragment
        for(int j = 0; j < fr_mem * 1024; j++)
            *((char*)(mem[i] + j)) = 0;
    }

//    // test
//    for (int i = 0; i < fr_mem * 1024; i++)
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
pair<int, int> fff_field()
{
    // finding fragment with some nodes already, but is not filled
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) < max_nodes_in_fragment && *((short*)(mem[i])) > 0 )
        {
            *((short*)(mem[i])) = *((short*)(mem[i])) + 1;
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
            *((short*)(mem[i])) = *((short*)(mem[i])) + 1;
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
int fff_fragment()
{
    for (int i = 0; i < fr_num; i++)
    {
        if ( *((short*)(mem[i])) == 0 )
        {
            *((short*)(mem[i])) = SHRT_MAX;
            *((void**)(mem[i] + fr_mem * 1024 - 4)) = NULL;
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
node* create_node(char* name, bool folder, node* parent, node* older_from, node* older_to)
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

    beg->indx = node_num;
    node_num++;

    // when created
    beg->younger = NULL;
    beg->start = NULL;
    beg->fragm = x.first;

    return beg;
}


/*
    Function for deleting node

    Input: pointer to node that should be deleted

    Output: /
*/
void delete_node(node* del)
{
    // printf("ind je %d\n", *((char*)(  (void*)del - 1)));
    // printf("prva dva bajta: %d\n", *((short*)(mem[0])));

    *((short*)(mem[ del->fragm ])) = *((short*)(mem[ del->fragm ])) - 1; // in this fragment is one node less
    *((char*)(  (void*)del - 1)) = 0; // is not filled with node anymore

    // printf("ind je %d\n", *((char*)(  (void*)del - 1)));
    // printf("prva dva bajta: %d\n", *((short*)(mem[0])));
}

/*
    Function for creating folder that is root and cannot be deleted.
*/
node* make_root()
{
    node* root = create_node("R", true, NULL, NULL, NULL);
    root->parent = root;
    root->older_from = root;
    return root;
}

/*
    List of commands with codes:
        exit - 1
        ls - 2
        cd - 3
        mkdir - 4
        touch - 5
        rm - 6
*/
void map_commands()
{
    mm["exit"] = 1;
    mm["ls"] = 2;
    mm["cd"] = 3;
    mm["mkdir"] = 4;
    mm["touch"] = 5;
    mm["rm"] = 6;
}

/*
    List of commands:
        exit
            code: 1
            ex: root exit
        ls (read-only) - List Directory - list everything that is inside selected directory
            code: 2
            ex: root ls
                list everything in folder root
        cd (read-only) - Change Directory - go in selected directory
            code: 3
            ex: root cd my-folder
                go to my-folder if it exists in folder root
        mkdir - Make Directory - make new directory
            code: 4
            ex: root mkdir my-folder
                make folder my-folder in folder root
        touch - making new file
            code: 5
            ex: root touch my-file.txt
                create new file whose name is 'my-file.txt'
        rm - Remove - delete selected file or folder
            code: 6
            ex: root rm my-folder
                delete my-folder and recursive everything inside
*/
void read_commands()
{
    map_commands();

    char command[10];
    bool running = true;

    do
    {
        scanf("%s", command);

        switch ( mm[command] )
        {
        case 1:
            {
                // exit
                running = false;
            } break;
        case 2:
            {
                // ls
            } break;
        case 3:
            {
                // cd
            } break;
        case 4:
            {
                // mkdir
            } break;
        case 5:
            {
                // touch
            } break;
        case 6:
            {
                // rm
            } break;
        default:
            {
                // wrong command
                printf("Wrong command!\n");
                // recommend help
            }
        }
    }
    while(running);
}

void test_fff_fragment()
{
    printf("%d\n", fff_fragment()); // 1

    for (int i = 1; i < 10; i++)
    {
        *((short*)(mem[i])) = 1;
    }
    printf("%d\n", fff_fragment()); // 10

    *((short*)(mem[4])) = max_nodes_in_fragment;
    *((short*)(mem[5])) = 0;
    *((short*)(mem[7])) = SHRT_MAX;
    printf("%d\n", fff_fragment()); // 5

    *((short*)(mem[5])) = SHRT_MAX;
    *((short*)(mem[10])) = 18;
    printf("%d\n", fff_fragment()); // 11

    for (int i = 1; i < fr_num - 1; i++)
    {
        *((short*)(mem[i])) = SHRT_MAX;
    }
    printf("%d\n", fff_fragment()); // 499

    for (int i = 1; i < fr_num; i++)
    {
        *((short*)(mem[i])) = SHRT_MAX;
    }
    printf("%d\n", fff_fragment()); // -1
}

void test_fff_field()
{
    pair<int, int> x;

    x = fff_field();
    printf("%d, %d\n", x.first, x.second); // 0, 1

    for (int i = 0; i < 60; i++)
        fff_field();
    x = fff_field();
    printf("%d, %d\n", x.first, x.second); // 1, 6

    for (int i = 0; i < 49; i++)
        fff_field();
    x = fff_field();
    printf("%d, %d\n", x.first, x.second); // 2, 0

    printf("%d\n", fff_fragment()); // 3
    for (int i = 0; i < 55; i++)
        fff_field();
    x = fff_field();
    printf("%d, %d\n", x.first, x.second); // 4, 0

    delete_node( (node*)(mem[0] + 3) );
    x = fff_field();
    printf("%d, %d\n", x.first, x.second); // 0, 3

    printf("\n");

    for (int i = 0; i < 1; i++)
    {
        printf("%d: %hd\n\n", i, *((short*)(mem[i])));
        for (int j = 2; j < fr_mem * 1024; j++)
        {
            if ( *((char*)(mem[i] + j)) != 0 )
                printf("%d: %d;\n", j, *((char*)(mem[i] + j)));
        }
        printf("\n");
    }
}

int main()
{
    initialize_mem();

    max_nodes_in_fragment = (fr_mem * 1024 - 2) / (sizeof(node) + 1);
    root = make_root();

    // testing
    // test_fff_fragment();
     test_fff_field();

    //read_commands();

    return 0;
}
