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

using namespace std;

/*
    List of commands with codes:
        exit - 1
        ls - 2
        cd - 3
        mkdir - 4
        touch - 5
        rm - 6
*/
void commands::map_commands()
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
void commands::read_commands()
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
                printf("2\n");
            } break;
        case 3:
            {
                // cd
                printf("3\n");
            } break;
        case 4:
            {
                // mkdir
                printf("4\n");
            } break;
        case 5:
            {
                // touch
                printf("5\n");
            } break;
        case 6:
            {
                // rm
                printf("6\n");
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
