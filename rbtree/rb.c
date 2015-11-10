/**********************************************
AUTHOR:zhd
TIME:Mon 09 Nov 2015 09:36:39 PM CST
FILENAME:rb.c
DESCRIPTION:.............
**********************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rb.h"

void *
rb_malloc (struct libavl_allocator *allocator, size_t size)
{
    assert (allocator != NULL && size > 0);
    return malloc (size);
}

void rb_free (struct libavl_allocator *allocator, void *block)
{
    assert (allocator != NULL && block != NULL);
    free (block);
}

struct libavl_allocator rb_allocator_default = {
    rb_malloc,
    rb_free
};

struct rb_table *
rb_create (rb_comparison_func *compare, void *param,
            struct libavl_allocator *allocator)
{
    struct rb_table *tree;

    assert (compare != NULL);

    if (allocator == NULL)
        allocator = &rb_allocator_default;

    tree = allocator->libavl_malloc (allocator, sizeof *tree);
    if (tree == NULL)
        return NULL;

    tree->rb_root = NULL;
    tree->rb_compare = compare;
    tree->rb_param = param;
    tree->rb_alloc = allocator;
    tree->rb_count = 0;
    tree->rb_generation = 0;

    return tree;
}

/**
 * 
 **/
void *
rb_find (const struct rb_table *tree, const void *item)
{
    const struct rb_node *p;

    assert (tree != NULL && item != NULL);
    for (p = tree->root; p != NULL; ) {
        int cmp = tree->rb_compare (item, p->rb_data, tree->rb_param);

        if (cmp < 0)
            p = p->rb_link[0];
        else if (cmp > 0)
            p = p->rb_link[1];
        else
            return p->rb_data;
    }
    return NULL;
}

void **
rb_probe (struct rb_table *tree, void *item)
{
    struct rb_node  *pa[RB_MAX_HEIGHT];
    unsigned char   da[RB_MAX_HEIGHT];
    int             k;

    struct rb_node *p;
    struct rb_node *n;

    assert (tree != NULL && item != NULL);

    pa[0] = (struct rb_node *) &tree->rb_root;
    da[0] = 0;
    k = 1;

    for (p = tree->rb_root; p != NULL; p = p->rb_link[da[k-1]]) {
        int cmp = tree->rb_compare (item, p->rb_data, tree->rb_param);
        if (cmp == 0)
            return &p->rb_data;

        pa[k] = p;
        da[k++] = cmp > 0;
    }

    n = pa[k-1]->rb_link[da[k-1]] = tree->rb_alloc->libavl_malloc (tree->rb_alloc, sizeof *n);
    if (n == NULL)
        return NULL;

    n->rb_data = item;
    n->rb_link[0] = n->rb_link[1] = NULL;
    n->rb_color = RB_RED;
    tree->rb_count++;
    tree->rb_generation++;

    while (k >= 3 && pa[k-1]->rb_color == RB_RED) {
        if (da[k-2] == 0) {
            struct rb_node *y = pa[k-2]->rb_link[1];
            
            if (y != NULL && y->rb_color == RB_RED) {
                pa[k-1]->rb_color = y->rb_color = RB_BLACK;
                pa[k-2]->rb_color = RB_RED;
                k -=2;
            } else {
                struct rb_node *x;

                if (da[k-1] != 0)
                    y = pa[k-1];
                else {
                    x = pa[k-1];
                    y = x->rb_link[1];
                    x->rb_link[1] = y->rb_link[0];
                    y->rb_link[0] = x;
                    pa[k-2]->rb_link[0] = y;
                }

                y = pa[k-2];
                x->rb_color = RB_RED;
                y->rb_color = RB_BLACK;

                x->rb_link[0] = y->rb_link[1];
                y->rb_link[1] = x;
                pa[k-3]->rb_link[da[k-3]] = y;

                break;
            }
        } else {
            struct rb_node *y = pa[k-2]->rb_link[0];
            
            if (y != NULL && y->rb_color == RB_RED) {
                pa[k-1]->rb_color = y->rb_color = RB_BLACK;
                pa[k-2]->rb_color = RB_RED;
                k -= 2;
            } else {
                struct rb_node *x;

                if (da[k-1] == 1)
                    y = pa[k-1];
                else {
                    x = pa[k-1];
                    y = x->rb_link[0];
                    x->rb_link[0] = y->rb_link[1];
                    y->rb_link[1] = x;
                    pa[k-2]->rb_link[1] = y;
                }

                x = pa[k-2];
                x->rb_color = RB_RED;
                y->rb_color = RB_BLACK;

                x->rb_link[1] = y->rb_link[0];
                y->rb_link[0] = x;
                pa[k-3]->rb_link[da[k-3]] = y;

                break;
            }
        }
    }
    
    tree->rb_root->rb_color = RB_BLACK;
    
    return &n->rb_data;
}

void *
rb_insert (struct rb_table *table, void *item)
{
    void **p = rb_probe (table, item);
    return (p == NULL || *p == item) ? NULL : *p;
}

void *
rb_replace (struct rb_table *table, void *item)
{
    void **p = rb_probe (table, item);

    if (p == NULL || *p == item)
        return NULL;
    else {
        void *r = *p;
        *p = item;
        return r;
    }
}





