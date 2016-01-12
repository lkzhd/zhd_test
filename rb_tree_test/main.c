/**********************************************
AUTHOR:zhd
TIME:Tue 12 Jan 2016 03:17:51 PM CST
FILENAME:main.c
DESCRIPTION:.............
**********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "rb.h"

#define MAX_BUF_LEN     32
#define MAX_NODE_NUM    100

typedef struct {
        int             index;
        char            *buf;
}zhd_rb_data_t;

static int      zhd_param = 0;

int
rb_node_compare (const void *node1, const void *node2, void *param)
{
        zhd_rb_data_t          *n1,*n2;
        int                     ret = 0;
        
        if (!node1 || !node2 || !param) {
                goto out;
        }

        n1 = (zhd_rb_data_t *)node1;
        n2 = (zhd_rb_data_t *)node2;

        if (n1->index < n2->index) {
                ret = -1;
        } else if (n1->index > n2->index) {
                ret = 1;
        } else {
                ret = 0;
        }
out:
        return ret;
}

int
main (int argc, char **argv)
{
        int                     ret = 0;
        zhd_rb_data_t           *data = NULL;
        zhd_rb_data_t           *getdata = NULL;
        zhd_rb_data_t           tmp = {0,};
        struct rb_table         *rbt = NULL;
        char                    buf[MAX_BUF_LEN] = {0};
        int                     i = 0;
        int                     index = 0;
        int                     randomfd = -1;
        char                    randomdata[50];
        size_t                  randomdata_len = 0;
        char                    *tmpfilename = NULL;


        rbt = rb_create (rb_node_compare, &zhd_param, NULL);

        for (;i < MAX_NODE_NUM;i++) {
                data = (zhd_rb_data_t *)malloc(sizeof (zhd_rb_data_t));
                if (!data) {
                        printf ("malloc error\n");
                        goto out;
                }
                data->index = i;
                sprintf(buf,"%d-XXXXXX",i);
                tmpfilename = mktemp(buf);
                data->buf = strdup(buf);
                memset (buf, 0, MAX_BUF_LEN);

                //printf("insert data:index is %d,%s\n",data->index,data->buf);
                rb_insert (rbt, data);
                if (!getdata) {
                        printf("rb_insert success\n");
                }
        }
        randomfd = open("/dev/random", O_RDONLY);
        if (randomfd == -1) {
                printf ("open /dev/random is error\n");
                goto out;
        }
        printf("get ten data randomly!\n");
        for (i = 0;i < 10;i++) {
                tmp.index = random()%MAX_NODE_NUM;
#if 0
                while (randomdata_len < sizeof randomdata) {
                        ssize_t         result;
                        result = read(randomfd, randomdata+randomdata_len,(sizeof randomdata - randomdata_len));
                        if (result < 0) {
                                /* error */
                                printf ("unable to read /dev/random\n");
                        }
                        randomdata_len += result;
                }
                printf("randomdata is %s\n",randomdata);
#endif
                getdata = rb_delete (rbt, &tmp);
                if (getdata) {
                        printf("get data:index is %d,%s\n",getdata->index,getdata->buf);
                        printf("get data:index is %d,%s\n",tmp.index,tmp.buf);
                } else {
                        printf("get data error\n");
                }

                getdata = rb_find (rbt, &tmp);
                if (getdata) {
                        printf("get data:index is %d,%s\n",getdata->index,getdata->buf);
                        printf("get data:index is %d,%s\n",tmp.index,tmp.buf);
                } else {
                        printf("get data error\n");
                }
        }
        if (randomfd > 0) {
                close(randomfd);
        }
out:
        return ret;
}



