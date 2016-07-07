#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include "../include/node.h"

// constants for fragments (KB)
#define full_mem 2000
#define fr_mem 4
#define fr_num full_mem/fr_mem

using namespace std;

void* mem[fr_num];
int node_num = 0;
map<string, int> mm;

void initialize_mem()
{
    for (int i = 0; i < fr_num; i++)
    {
        mem[i] = malloc(fr_mem * 1024);

//        for(int j=0; j<fr_mem*1024; j++) *((char*)(mem[i] + j)) = 5;

        *((short*)(mem[i] + 0)) = (short)0; // free fragment
    }

//    for (int i = 0; i < fr_mem*10; i++)
//    {
//        printf("%d\n", *((char*)(mem[8] + i)));
//    }
}

node* create_node(char* name, bool folder, node* parent, node* older_from)
{
    // finding first free field of free memory
    // sample: first fragment
    node* beg = (node*)(mem[0] + 2); // beginning of this node

    // given parameters
    strcpy(beg->name, name);
    beg->folder = folder;
    beg->parent = parent;
    beg->older_from = older_from;

    beg->indx = node_num;
    node_num++;

    // when created
    beg->older_to = NULL;
    beg->younger = NULL;
    beg->start = NULL;

    return beg;
}

void delete_node(node* del)
{
    // deleting
}

node* make_root()
{
    node* root = create_node("R", true, NULL, NULL);
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

int main()
{
    initialize_mem();
    node* root = make_root();
    read_commands();

    return 0;
}
