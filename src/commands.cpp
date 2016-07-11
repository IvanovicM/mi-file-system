/*
    Marina Ivanovic
    Project in Petnica Science Center
*/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>

#include "../include/commands.h"
#include "../include/node.h"
#include "../include/memory.h"

using namespace std;

void commands::input_error()
{
    // wrong command
    printf("Wrong command!\n");
    // recommend help
}

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
        nxt->print_node();
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
        {
            if (!(nxt->folder))
            {
                input_error();
                return curr;
            }
            else
            {
                return nxt;
            }
        }
        nxt = nxt->older_to;
    }

    printf("No such file or directory.\n");
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

void commands::_copy(node* from, node* cop, node* to)
{

}

/*
    Prints path before scanning command;
*/
void commands::print_path(node* curr, node* root)
{
    if (curr != root)
    {
        print_path(curr->parent, root);
        printf("\\%s", curr->name);
    }
    else
        printf("%s:", curr->name);
}

/*
    List of commands with codes:
        exit - 1
        ld - 2
        cd - 3
        mkdir - 4
        mkfile - 5
        del - 6
        cp - 7
*/
void commands::map_commands()
{
    mm["exit"] = 1;
    mm["ld"] = 2;
    mm["cd"] = 3;
    mm["mkdir"] = 4;
    mm["mkfile"] = 5;
    mm["del"] = 6;
    mm["cp"] = 7;
}

/*
    List of commands:
        exit
            code: 1
            ex: root> exit
        ld (read-only) - List Directory - list everything that is inside selected directory
            code: 2
            ex: root> ld
                list everything in folder root
        cd (read-only) - Change Directory - go to selected directory
            code: 3
            ex: root> cd my-folder
                go to my-folder if it exists in folder root
        mkdir - Make Directory - make new directory
            code: 4
            ex: root> mkdir my-folder
                make folder my-folder in folder root
        mkfile - Make File
            code: 5
            ex: root> mkfile my-file.txt
                create new file whose name is 'my-file.txt'
        del - Delete - delete selected file or folder
            code: 6
            ex: root> del my-folder
                delete my-folder and recursive everything inside
        cp - Copy
            code: 7
            ex: root> cp file.txt root/a/b
                copies file.txt that was in root to folder root/a/b
*/
void commands::read_commands(memory* part, node* curr, node* root)
{
    map_commands();

    bool running = true;

    do
    {
        // print path
        print_path(curr, root);
        printf("> ");

        // scan
        string line;
        getline(cin, line);

        // parse
        istringstream iss(line);
        string cmd;
        iss >> cmd;

        // recognize command
        switch ( mm[cmd] )
        {
        case 1: // exit
            {
                int uk = 0;
                while (iss)
                {
                    string sub;
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                    running = false;
                else
                    input_error();

            } break;
        case 2: // list directory
            {
                int uk = 0;
                while (iss)
                {
                    string sub;
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                    _list_directory(curr);
                else
                    input_error();

            } break;
        case 3: // change directory
            {
                char name[40];
                string sub;
                iss >> sub;
                strcpy(name, sub.c_str());

                int uk = 0;
                while (iss)
                {
                    sub;
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                    curr = _change_directory(curr, name);
                else
                    input_error();

            } break;
        case 4: // make directory
            {
                string sub;
                iss >> sub;
                if (sub.length() > 40)
                    printf("Invalid name.\n");
                else
                {
                    char name[40];
                    strcpy(name, sub.c_str());

                    int uk = 0;
                    while (iss)
                    {
                        sub;
                        iss >> sub;
                        uk++;
                    }

                    if (uk == 1)
                    {
                        if ( (strstr(name, "\\") || strstr(name, "/") || strstr(name, ":") || strstr(name, "*") || strstr(name, "?") || strstr(name, "\"") || strstr(name, "<") || strstr(name, ">") || strstr(name, "|")) || (!strcmp(name, ".") || !strcmp(name, "..")) )
                            printf("Invalid name.\n");
                        else
                        {
                            bool found = false;

                            node* nxt = curr->younger;
                            while (nxt != NULL && !found)
                            {
                                if (!strcmp(nxt->name, name))
                                    found = true;
                                nxt = nxt->older_to;
                            }

                            if (found)
                                printf("Directory or file with this name already exists.\n");
                            else
                            {
                                _make_directory(part, curr, name);
                            }
                        }
                    }
                    else
                        input_error();
                }
            } break;
        case 5: // make file
            {
                string sub;
                iss >> sub;
                if (sub.length() > 40)
                    printf("Invalid name.\n");
                else
                {
                    char name[40];
                    strcpy(name, sub.c_str());

                    int uk = 0;
                    while (iss)
                    {
                        sub;
                        iss >> sub;
                        uk++;
                    }

                    if (uk == 1)
                    {
                        if ( (strstr(name, "\\") || strstr(name, "/") || strstr(name, ":") || strstr(name, "*") || strstr(name, "?") || strstr(name, "\"") || strstr(name, "<") || strstr(name, ">") || strstr(name, "|")) || (!strcmp(name, ".") || !strcmp(name, "..")) )
                            printf("Invalid name.\n");
                        else
                        {
                            bool found = false;

                            node* nxt = curr->younger;
                            while (nxt != NULL && !found)
                            {
                                if (!strcmp(nxt->name, name))
                                    found = true;
                                nxt = nxt->older_to;
                            }

                            if (found)
                                printf("Directory or file with this name already exists.\n");
                            else
                            {
                                _make_file(part, curr, name);
                            }
                        }
                    }
                    else
                        input_error();
                }
            } break;
        case 6: // delete
            {
                char name[40];
                string sub;
                iss >> sub;
                strcpy(name, sub.c_str());

                int uk = 0;
                while (iss)
                {
                    sub;
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                    {
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
                    }
                else
                    input_error();
            } break;
        case 7: // copy
            {

            }
        default:
            {
                input_error();
            }
        }
    }
    while(running);
}
