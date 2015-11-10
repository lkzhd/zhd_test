/**********************************************
AUTHOR:zhd
TIME:Tue 10 Nov 2015 09:42:19 PM CST
FILENAME:inotify_test.c
DESCRIPTION:.............
**********************************************/
#include <stdio.h>
#include <sys/inotify.h>
#include <limits.h>
#include <stdlib.h>

#define NOTIFY_EVENT_NUM (8 * (sizeof (struct inotify_event) + NAME_MAX + 1))

int
main (int argc, char **argv)
{
    int                     notify_fd = 0, watch_fd = 0;
    int                     ret = -1;
    uint32_t                mask = 0;
    char                    notify[NOTIFY_EVENT_NUM] = {0,};
    char                    *p = NULL;
    struct inotify_event    *event = NULL;
    int                     num = 0;
    int                     index = 0;

    if (argc != 2) {
        printf ("argc should be 2\n");
        goto out;
    }
    printf ("inotify_init.\n");
    notify_fd = inotify_init ();
    if (-1 == notify_fd) {
        printf ("inotify_init failed!\n");
        goto out;
    }

    mask |= IN_ALL_EVENTS;

    printf ("add %s with mask: %x as watch fd.\n", argv[1], mask);

    watch_fd = inotify_add_watch (notify_fd, argv[1], mask);
    if (-1 == watch_fd) {
        printf ("inotify_add_watch failed!\n");
        goto out;
    }

    printf ("now waiting for file change...\n");
    
    while (1) {
        ret = read (notify_fd, notify, NOTIFY_EVENT_NUM);
        if (0 == ret) {
            printf ("read failed.\n");
        }

        if (-1 == ret) {
            printf ("read failed and exit.\n");
            goto out;
        }

        for (p = notify; p < notify + ret; ) {
            event = (struct inotify_event *) p;
            /**
             * TODO:display event infomation here.
             **/
            p += sizeof (struct inotify_event) + event->len;
        }

    }
    close (notify_fd);
out:
    return ret;
}


