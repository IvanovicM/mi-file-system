/*
    Marina Ivanovic
    Project in Petnica Science Center
*/

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>

#include "../include/commands.h"
#include "../include/node.h"
#include "../include/memory.h"

using namespace std;

/*
    ld
*/
void commands::_list_directory(node* curr)
{
    node* nxt = curr->younger;
    if (nxt == NULL)
        printf("Empty directory.\n");
    while (nxt != NULL)
    {
        printf("%s\n", nxt->name);
        nxt = nxt->older_to;
    }
}

/*
    cd
*/
node* commands::_change_directory(node* curr, char* name)
{
    if (!strcmp(name, "..")) // go back
        return curr->parent;

    node* nxt = curr->younger;
    while (nxt != NULL) // finding that directory
    {
        if (!strcmp(name, nxt->name))
            return nxt;
        nxt = nxt->older_to;
    }

    printf("No such file or directory\n");
    return curr;
}

/*
    mkdir
*/
void commands::_make_directory(memory* part, node* curr, char* name)
{
    node* nxt = part->create_node(name, true, curr, curr, curr->younger);
    curr->younger = nxt;

//    // test
//    printf("name: %s\nfolder: %s\n", nxt->name, nxt->folder ? "yes" : "no");
}

/*
    mkfile
*/
void commands::_make_file(memory* part, node* curr, char* name)
{
    node* nxt = part->create_node(name, false, curr, curr, curr->younger);
    curr->younger = nxt;

//    // test
//    printf("name: %s\nfolder: %s\n", nxt->name, nxt->folder ? "yes" : "no");
}

/*
    del
*/
void commands::_delete(memory* part, node* curr)
{
    node* nxt = curr->younger;
    while (nxt != NULL)
    {
        part->delete_node(nxt);
        nxt = nxt->older_to;
    }
    part->delete_node(curr);
}

/*
    List of commands with codes:
        exit - 1
        ld - 2
        cd - 3
        mkdir - 4
        mkfile - 5
        del - 6
*/
void commands::map_commands()
{
    mm["exit"] = 1;
    mm["ld"] = 2;
    mm["cd"] = 3;
    mm["mkdir"] = 4;
    mm["mkfile"] = 5;
    mm["del"] = 6;
}

/*
    List of commands:
        exit
            code: 1
            ex: root exit
        ld (read-only) - List Directory - list everything that is inside selected directory
            code: 2
            ex: root ld
                list everything in folder root
        cd (read-only) - Change Directory - go in selected directory
            code: 3
            ex: root cd my-folder
                go to my-folder if it exists in folder root
        mkdir - Make Directory - make new directory
            code: 4
            ex: root mkdir my-folder
                make folder my-folder in folder root
        mkfile - Make File
            code: 5
            ex: root mkfile my-file.txt
                create new file whose name is 'my-file.txt'
        del - Delete - delete selected file or folder
            code: 6
            ex: root del my-folder
                delete my-folder and recursive everything inside
*/
void commands::read_commands(memory* part, node* curr)
{
    map_commands();

    char command[10];
    bool running = true;

    do
    {
        printf("%s> ", curr->name);
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
                // ld
                _list_directory(curr);
            } break;
        case 3:
            {
                // cd
                char name[40];
                scanf("%s", name);
                curr = _change_directory(curr, name);
            } break;
        case 4:
            {
                // mkdir
                char name[40];
                scanf("%s", name);
                _make_directory(part, curr, name);
            } break;
        case 5:
            {
                // mkfile
                char name[40];
                scanf("%s", name);
                _make_file(part, curr, name);
            } break;
        case 6:
            {
                // delete
                char name[40];
                scanf("%s", name);

                node* nxt = curr->younger;
                bool found = false;
                while (nxt != NULL && !found) // finding that directory
                {
                    if (!strcmp(name, nxt->name))
                        {
                            _delete(part, nxt);
                            found = true;
                        }
                    else
                        nxt = nxt->older_to;
                }

                if (!found)
                    printf("No such file or directory.\n");
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
