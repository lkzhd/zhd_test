/**********************************************
AUTHOR:zhd
TIME:Mon 09 Nov 2015 11:12:13 PM CST
FILENAME:bucket_sort.c
DESCRIPTION:.............
**********************************************/

#include <stdio.h>
#include <assert.h>

#define MAX_NUM 100

void 
bucket_sort(int *input, int len, int *output)
{
    int     index;
    int     *out = output;
    
    assert(input != NULL && output != NULL && len > 0);
    for (index = 0; index < len; index++) {
        out[index] == 0;
    }
    for (index = 0; index < len; index++) {
        out[input[index]]++;
    }
}

int
main()
{
    int     in[10] = {1,20,23,4,53,4,3,4,4,0};
    int     out[MAX_NUM] = {0};
    int     index = 0;
    int     i = 0;

    bucket_sort(in,10,out);

    printf ("After sort:");
    for (index; index < MAX_NUM; index++) {
        for (i = 0; i < out[index]; i++) {
            printf("%d ",index);
        }
    }
    printf ("\n");

    printf ("\b");
}

