#ifndef __RB_TREE__
#define __RB_TREE__

typedef enum {
    RB_RED,
    RB_BLACK
}rb_color_t;

typedef struct rb_node {
    void            *data;
    rb_color_t      color;
    struct rb_node  *left;
    struct rb_node  *right;
}rb_node_t;

typedef struct rb_allocator {
    rb_node_t   *alloc (char *);
    void        free(rb_node_t *);
}rb_allocator_t;

typedef struct rb_table {
    rb_node_t       *root;

}

#endif
