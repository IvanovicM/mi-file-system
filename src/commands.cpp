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
#include <time.h>

#include "../include/commands.h"
#include "../include/node.h"
#include "../include/memory.h"

using namespace std;

node* dest_prnt;
string dest_name;
bool same;

/*
    Inform if input is not correct.
*/
void commands::input_error()
{
    // wrong command
    printf("Wrong command!\n");
    // recommend help
}

/*
    Find son of given node, if it exists.
*/
node* commands::find_son(node* parent, char* son_name)
{
    if (parent == NULL)
        return NULL;

    if (!strcmp(son_name, "..")) // go back
        return parent->parent;

    if (!strcmp(son_name, "."))
        return parent;

    node* nxt = parent->younger;
    while (nxt != NULL) // finding that directory or file
    {
        if (!strcmp(son_name, nxt->name))
        {
            return nxt;
        }
        nxt = nxt->older_to;
    }
    return NULL;
}

/*
    Parse string and recognize folder (if possible).
*/
void commands::destination(memory* part, node* root, node* curr, string dest)
{
    string delimiter = "/";

    size_t pos = 0;
    string token;

    // check if begging is root
    pos = dest.find(delimiter);
    token = dest.substr(0, pos);
    if (token.empty())
    {
        curr = root;
        dest.erase(0, pos + delimiter.length());
    }

    while ((pos = dest.find(delimiter)) != string::npos)
    {
        token = dest.substr(0, pos);
        if (strcmp(token.c_str(), ".")) // do not stay in the same directory
        {
            if (strcmp(token.c_str(), "..")) // go to parent
            {
                curr = find_son(curr, (char*)token.c_str());
                if (curr == NULL || !curr->folder)
                {
                    dest_prnt = NULL;
                    dest_name = dest;
                    return;
                }
            }
            else
                curr = curr->parent;
        }
        dest.erase(0, pos + delimiter.length());
    }

    dest_prnt = curr;
    dest_name = dest;
}

/*
    ld
*/
void commands::_list_directory(node* curr)
{
    if (curr == NULL)
        {
            input_error();
            return;
        }

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
node* commands::_change_directory(node* curr)
{
    if (curr == NULL)
        return NULL;

    if (!curr->folder)
        return NULL;

    return curr;
}

/*
    mkdir
*/
void commands::_make_directory(memory* part, node* curr, string sub)
{
    if (curr == NULL)
    {
        input_error();
        return;
    }

    if (sub.length() > 40)
        printf("Invalid name.\n");
    else
    {
        char name[40];
        strcpy(name, sub.c_str());

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
                node* nxt = part->create_node(name, true, curr, curr, curr->younger);
                curr->younger = nxt;
            }
        }
    }
}

/*
    mkfile
*/
void commands::_make_file(memory* part, node* curr, string sub)
{
    if (curr == NULL)
    {
        input_error();
        return;
    }

    if (sub.length() > 40)
        printf("Invalid name.\n");
    else
    {
        char name[40];
        strcpy(name, sub.c_str());

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
                node* nxt = part->create_node(name, false, curr, curr, curr->younger);
                curr->younger = nxt;
            }
        }
    }
}

/*
    del
*/
void commands::_delete(memory* part, node* root, node* curr, node* del)
{
    if (del == curr) // deleting directory that is current directory
        same = false;

    if (del == NULL)
    {
        input_error();
        return;
    }

    if (del == root)
    {
        printf("Must not delete this directory!\n");
        return;
    }

    node* nxt = del->younger;
    while (nxt != NULL)
    {
        _delete(part, root, curr, nxt);
        nxt = nxt->older_to;
    }

    part->delete_node(del);
}

/*
    cp -ext

    Copy extern file to the current node.
*/
void commands::_cp_extern_file(memory* part, node* curr, char* extern_file_name)
{
    FILE* file = fopen(extern_file_name, "rb"); // read binary

    if (file == NULL)
    {
        printf("This file does not exist.\n");
        part->delete_node(curr);
        return;
    }

    // delete file
    part->delete_file(curr);

    int buffer_size = 512; // 512B
    char* buffer = new char[buffer_size];
    int vl;
    while (vl = fread(buffer, 1, buffer_size, file))
    {
        part->add_extern_file(curr, buffer, vl);
    }

    delete[] buffer;

    curr->date_modf = time(0); // last time updated
}

/*
    cp -int

    Copy file from this file system to the current node.
*/
void commands::_cp_intern_file(memory* part, node* from, node* curr)
{
    if (from->folder)
    {
        printf("This is not a file.\n");
        part->delete_node(curr);
        return;
    }

    // delete file
    part->delete_file(curr);
    part->add_intern_file(from, curr);

    curr->date_modf = time(0); // last time updated
}

/*
    Prints path before scanning command.
*/
void commands::print_path(node* curr, node* root)
{
    if (curr != root)
    {
        print_path(curr->parent, root);
        printf("/%s", curr->name);
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
                string dest;
                string sub;
                iss >> sub;
                dest = sub;

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 0)
                    _list_directory(curr); // list current directory
                else
                {
                    if (uk == 1) // list directory from path
                    {
                        destination(part, root, curr, dest);
                        _list_directory( find_son(dest_prnt, (char*)dest_name.c_str()) );
                    }
                    else
                        input_error();
                }

            } break;
        case 3: // change directory
            {
                string dest;
                string sub;
                iss >> sub;
                dest = sub;

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                {
                    destination(part, root, curr, dest);
                    node* nxt;
                    if ( (nxt = _change_directory( find_son(dest_prnt, (char*)dest_name.c_str()) ) ) == NULL )
                        input_error();
                    else
                        curr = nxt;
                }
                else
                    input_error();

            } break;
        case 4: // make directory
            {
                string dest;
                string sub;
                iss >> sub;
                dest = sub;

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                {
                    destination(part, root, curr, dest);
                    _make_directory(part, dest_prnt, dest_name);
                }
                else
                    input_error();

            } break;
        case 5: // make file
            {
                string dest;
                string sub;
                iss >> sub;
                dest = sub;

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                {
                    destination(part, root, curr, dest);
                    _make_file(part, dest_prnt, dest_name);
                }
                else
                    input_error();

            } break;
        case 6: // delete
            {
                string dest;
                string sub;
                iss >> sub;
                dest = sub;

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                {
                    destination(part, root, curr, dest);
                    node* nxt;
                    nxt = find_son(dest_prnt, (char*)dest_name.c_str());
                    same = true;
                    _delete(part, root, curr, nxt);
                    if (!same)
                        curr = nxt->parent;
                }
                else
                    input_error();

            } break;
        case 7: // copy
            {
                string sub;
                string option;
                string file;
                string dest;

                iss >> sub;
                option = sub;

                if (iss)
                {
                    iss >> sub;
                    file = sub;
                }

                if (iss)
                {
                    iss >> sub;
                    dest = sub;
                }

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 1)
                {
                    if (option == "-ext")
                    {
                        destination(part, root, curr, dest);
                        node* nd = find_son(dest_prnt, (char*)dest_name.c_str());
                        if (nd == NULL) // should make new folder
                        {
                            _cp_extern_file(part, part->create_node((char*)dest_name.c_str(), false, dest_prnt, dest_prnt, dest_prnt->younger), (char*)file.c_str());
                        }
                        else
                        {
                            // not specified what file to copy to
                            input_error();
                        }

                    }
                    else
                    {
                        input_error();
                    }
                }
                else
                {
                    if (option == "-int")
                    {
                        destination(part, root, curr, dest);
                        node* from = find_son(dest_prnt, (char*)dest_name.c_str());
                        if (from != NULL || nd->falder)
                        {
                            // file that exists
                            destination(part, root, curr, file);
                            node* to = find_son(dest_prnt, (char*)dest_name.c_str());
                            if (to != NULL || to->folder)
                            {
                                node* crt = part->create_node(from->name, false, to, to, to->younger);
                                add_intern_file(from, crt);
                            }
                            else
                            {
                                if (to == NULL)
                                {
                                    node* crt = part->create_node((char*)dest_name.c_str(), false, dest_prnt, dest_prnt, dest_prnt->younger);
                                    add_intern_file(from, crt);
                                }
                                else
                                {
                                    input_error();
                                }
                            }
                        }
                        else
                        {
                            // want to copy file that does not exist or is a directory
                            input_error();
                        }
                    }
                    else
                        input_error();
                }
            } break;
            default:
            {
                input_error();
            }
        }

        printf("\n");
    }
    while(running);
}
