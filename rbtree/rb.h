#ifndef RB_H
#define RB_H 1

#include <stddef.h>

typedef int rb_comparison_func (const void *rb_a, const void *rb_b,
                                void *rb_param);

typedef void rb_item_func (void *rb_item, void *rb_param);

typedef void *rb_copy_func (void *rb_item, void *rb_param);

#ifndef LIBAVL_ALLOCATOR
#define LIBAVL_ALLOCATOR

struct libavl_allocator {
        void *(*libavl_malloc) (struct libavl_allocator *, size_t libavl_size);
        void (libavl_free) (struct libavl_allocator *, void *libavl_block);
};
#endif

extern struct libavl_allocator rb_allocator_default;
void *rb_malloc (struct libavl_allocator *, size_t);
void rb_free (struct libavl_allocator *, void *);

#ifndef RB_MAX_HEIGHT
#define RB_MAX_HEIGHT 128
#endif

struct rb_table {
        struct rb_node          *rb_root;
        rb_comparison_func      *rb_compare;
        void                    *rb_param;
        struct libavl_allocator *rb_alloc;
        size_t                  rb_count;
        unsigned long           rb_generation;
};

enum rb_color {
        RB_BLACK,
        RB_RED
};

struct rb_node {
        struct rb_node          *rb_link[2];
        void                    *rb_data;
        unsigned char           rb_color;
};

struct rb_traverser {
    struct rb_table             *rb_table;
    struct rb_node              *rb_node;
    struct rb_node              *rb_stack[RB_MAX_HEIGHT];

    size_t                      rb_rb_height;
    unsigned long               rb_generation;
};

struct rb_table *rb_create (rb_comparison_func *, void *,
                            struct libavl_allocator *);
struct rb_table *rb_copy (const struct rb_table *,rb_copy_func *,
                            rb_item_func *, struct libavl_allocator *);

void rb_destroy (struct rb_table *, rb_item_func *);
void **rb_probe (struct rb_table *, void *);
void *rb_insert (struct rb_table *,void *);
void *rb_replace (struct rb_table *, void *);
void *rb_delete (struct rb_table *, const void *);
void *rb_find (const struct rb_table *,const void *);
void rb_assert_insert (struct rb_table *, void *);
void *rb_assert_delete (struct rb_table *,void *)；

/* Table traverser functions */
void rb_t_init (struct rb_traverser *, struct rb_table *);
void *rb_t_first (struct rb_traverser *, struct rb_table *);
void *rb_t_last (struct rb_traverser *, struct rb_table *);
void *rb_t_find (struct rb_traverser *, struct rb_table *, void *);
void *rb_t_insert (struct rb_traverser *, struct rb_table *, void *);
void *rb_t_copy (struct rb_traverser *, struct rb_traverser *);
void *rb_t_next (struct rb_traverser *);
void *rb_t_prev (struct rb_traverser *);
void *rb_t_cur (struct rb_traverser *);
void *rb_t_replace (struct rb_traverser *, void *);

#endif /* rb.h */
