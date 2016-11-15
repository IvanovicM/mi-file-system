/*
    Marina Ivanovic
    Project in Petnica Science Center
    year: 2016
*/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <string>
#include <string.h>
#include <time.h>
#include <ctime>

#include "../include/commands.h"
#include "../include/node.h"
#include "../include/memory.h"

#define buffer_size 10000 // B

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
    printf("For help type \"help\".\n");
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

    char* buffer = new char[buffer_size];
    int vl = fread(buffer, 1, buffer_size, file);
    int crfr;
    int frnum = 0;
    if (vl) // if file is not empty
        {
            curr->start = part->fff_fragment();
            crfr = curr->start;
        }
    while (vl)
    {
        pair<int, int> x = part->add_extern_file(curr, buffer, vl, crfr, frnum);
        crfr = x.first;
        frnum = x.second;
        vl = fread(buffer, 1, buffer_size, file);
    }

    delete[] buffer;

    fclose(file);

    curr->date_modf = time(0); // last time updated
}

/*
    cp -int

    Copy file from this file system to the current node.
*/
void commands::_cp_intern_file(memory* part, node* from, node* to)
{
    if (from->folder)
    {
        printf("This is not a file.\n");
        part->delete_node(to);
        return;
    }

    // delete file
    part->delete_file(to);
    part->add_intern_file(from, to);

    to->date_modf = time(0); // last time updated
}

/*
    Help user to use commands.
*/
void commands::__help(string cmd)
{
    switch ( mm[cmd] )
    {
    case 1: // exit
        {
            printf("If you want to quit type \"exit\".\n");
        }break;
    case 2: // ld
        {
            printf("ld [directory_name]\n");
            printf("ld [path][directory_name]\n");
        }break;
    case 3: // cd
        {
            printf("cd [..]\n");
            printf("cd [directory_name]\n");
            printf("cd [path][directory_name]\n");
        }break;
    case 4: // mkdir
        {
            printf("mkdir [directory_name]\n");
            printf("mkdir [path][directory_name]\n");
        }break;
    case 5: // mkfile
        {
            printf("mkfile [file_name]\n");
            printf("mkfile [path][file_name]\n");
        }break;
    case 6: // del
        {
            printf("del [directory/file_name]\n");
            printf("del [path][directory/file_name]\n");
        }break;
    case 7: // cp
        {
            printf("%-10s If you want to copy from extern file.\n", "cp -ext");
            printf("cp -int [extern_file_name] [path][file_name]\n");

            printf("\n");

            printf("%-10s If you want to copy from intern file.\n", "cp -int");
            printf("cp -int [path][file_name] [path][file_name]\n");
            printf("cp -int [path][file_name] [path][directory_name]\n");
        }break;
    case 8: // pt
        {
            printf("pt [file_name]\n");
            printf("pt [path][file_name]\n");
        }break;
    case 9: // help
        {
            printf("For more information on a specific command, type \"help command-name\".\n");
            printf("%-10s Changes the current directory.\n", "cd");
            printf("%-10s Copies one file to another location.\n", "cp");
            printf("%-10s Deletes one file.\n", "del");
            printf("%-10s Quits.\n", "exit");
            printf("%-10s Displays a list of files and subdirectories in a directory.\n", "ld");
            printf("%-10s Creates an empty directory.\n", "mkdir");
            printf("%-10s Creates an empty file.\n", "mkfile");
            printf("%-10s Prints a text file.\n", "pt");
        }break;
    default:
        {
            input_error();
        }
    }
}

/*
    Print path before scanning command.
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
    Print file.
*/
void commands::print_file(memory* part, node* curr)
{
    if (curr->folder)
    {
        printf("This is not a file.\n");
        return;
    }

    int frsz = fr_mem * 1024 - 2 - 4; // fr_mem - sizeof(short) - sizeof(int);
    int from_fr = curr->start;
    int sz = curr->sizeB;

    //node; optional
    // curr->print_node();

    // file
    while (from_fr != 0 && sz)
    {
        for (int i = 0; i < frsz && sz; i++)
        {
            printf("%c", *((char*)(part->mem[from_fr] + 2 + i)) );
            sz--;
        }

        from_fr = *((int*)( (char*)(part->mem[from_fr] + fr_mem * 1024 - 4)) );
    }

    printf("\n");
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
        pt - 8
        help - 9
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
    mm["pt"] = 8;
    mm["help"] = 9;
}

/*
    List of commands:
        exit
            code: 1
            ex: R> exit
        ld (read-only) - List Directory - list everything that is inside selected directory
            code: 2
            ex: R> ld
                list everything in folder R
        cd (read-only) - Change Directory - go to selected directory
            code: 3
            ex: R> cd my-folder
                go to my-folder if it exists in folder R
        mkdir - Make Directory - make new directory
            code: 4
            ex: R> mkdir my-folder
                make folder my-folder in folder R
        mkfile - Make File
            code: 5
            ex: R> mkfile my-file.txt
                create new file whose name is 'my-file.txt'
        del - Delete - delete selected file or folder
            code: 6
            ex: R> del my-folder
                delete my-folder and recursive everything inside
        cp - Copy
            code: 7
            ex: R> cp -ext D:\FS\from.txt /a/b
                copies extern file to folder /a/b/file
                R> cp -ext D:\FS\from.txt /a/b
                copies extern file to file /a/b/file
        pt - Print
            code: 8
            ex:
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

        clock_t begin_t, end_t;
        begin_t = clock();

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

                // regognize option
                iss >> sub;
                option = sub;

                // regognize file that should be copied
                if (iss)
                {
                    iss >> sub;
                    file = sub;
                }

                // regognize destination that file should be copied to
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
                        node* to = find_son(dest_prnt, (char*)dest_name.c_str());
                        if (dest_prnt != NULL && to == NULL) // should make new file in existing directory
                        {
                            _make_file(part, dest_prnt, dest_name);
                            node* cf = find_son(dest_prnt, (char*)dest_name.c_str());
                            _cp_extern_file(part, cf, (char*)file.c_str());
                        }
                        else
                        {
                            // not specified what file to copy to
                            input_error();
                        }
                    }
                    else
                    {
                        if (option == "-int")
                        {
                            destination(part, root, curr, file);
                            node* from = find_son(dest_prnt, (char*)dest_name.c_str());
                            if (from != NULL && !from->folder)
                            {
                                // file that exists
                                destination(part, root, curr, dest);
                                node* to = find_son(dest_prnt, (char*)dest_name.c_str());
                                if (to != NULL && to->folder) // should copy to directory 'to'
                                {
                                    _make_file(part, to, from->name);
                                    node* cf = find_son(to, from->name);
                                    part->add_intern_file(from, cf);
                                }
                                else
                                {
                                    if (to == NULL) // should copy to file 'to'
                                    {
                                        _make_file(part, dest_prnt, dest_name);
                                        node* cf = find_son(dest_prnt, (char*)dest_name.c_str());
                                        part->add_intern_file(from, cf);
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
                        {
                            input_error();
                        }
                    }
                }
                else
                    {
                        input_error();
                    }
            } break;
        case 8: // print
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
                    if ( ( nxt = find_son(dest_prnt, (char*)dest_name.c_str()) ) == NULL )
                        input_error();
                    else
                        print_file(part, nxt);
                }
                else
                    input_error();
            } break;
        case 9: // help
            {
                string hp_cmd;
                string sub;
                iss >> sub;
                hp_cmd = sub;

                int uk = 0;
                while (iss)
                {
                    iss >> sub;
                    uk++;
                }

                if (uk == 0)
                    __help("help"); // list current directory
                else
                {
                    if (uk == 1 && hp_cmd != "help") // list directory from path
                    {
                        __help(hp_cmd);
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

        end_t = clock();
        double dif = double(end_t - begin_t) / CLOCKS_PER_SEC;
        printf ("\nElasped time is %.2lf seconds.\n", dif);

        printf("\n");
    }
    while(running);
}
