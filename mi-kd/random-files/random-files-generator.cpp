#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <iostream>

#define files_num 10
#define file_length 100
#define LO 0
#define HI 100

using namespace std;

int main()
{
    FILE* sfile;
    sfile = fopen("files.txt", "w");
    fprintf(sfile, "%d\n", files_num);
    for (int i = 0; i < files_num; i++)
    {
        int k = i + 1;
        string num;
        while (k)
        {
            num.push_back('0' + k%10);
            k/= 10;
        }
        int n = num.length();
        for (int j = 0; j < n / 2; j++)
        {
            char p = num[j];
            num[j] = num[n-1-j];
            num[n-1-j] = p;
        }
        //reverse(num.begin(), num.end());

        string name = "file" + num + ".txt";

        FILE* fl;
        fl = fopen(name.c_str(), "w");
        for (int j = 0; j < file_length; j++)
        {
            //srand(time(0));
            //fprintf(fl, "%d", rand());
            fprintf(fl, "%d", i);
        }

        fprintf(sfile, "%f %f\n", LO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HI-LO))), LO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HI-LO))));
        fprintf(sfile, "object%d\n", i);
        fprintf(sfile, "D:\\random-files\\%s\n", name.c_str());

        fclose(fl);
    }

    fclose(sfile);

    return 0;
}
