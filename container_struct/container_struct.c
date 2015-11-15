/**********************************************
AUTHOR:zhd
TIME:Sun 15 Nov 2015 10:47:09 PM CST
FILENAME:container_struct.c
DESCRIPTION:.............
**********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


typedef int (*compare_data_t)(void*, void*);

typedef struct test_ {
    void            *data;
    char            index;
    compare_data_t  fun;
}test_t;

#define container_of(ptr, type, member) ({\
        const typeof ((((type *)0)->member)) *__mptr = ptr;\
        (type *)((char *)__mptr - offsetof (type, member));})

int
main()
{
    int             ret = -1;
    test_t          *test = NULL;

    test = (test_t *) malloc(sizeof (test_t));

    printf ("test_t struct pointer is : %p\n", test);
    printf ("test_t struct pointer is : %p\n", container_of(&(test->index), test_t, index));


    return ret;
}



