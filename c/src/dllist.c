/*******************************************************************************
 * File         : dllist.c
 * Author(s)    : Tekin Ozbek <tekin@tekinozbek.com>
 ******************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <dllist.h>

static dlnode_t* get_node(dllist_t* l, size_t pos) {

    dlnode_t* node;
    size_t i;

    if (l->length == 0)
        return &l->dummy;

    if (pos < l->length / 2) {

        node = l->dummy.next;
        for (i = 0; i < pos; ++i)
            node = node->next;
    }

    else {

        node = &l->dummy;
        for (i = l->length; i > pos; --i)
            node = node->prev;
    }

    return node;
}

void dllist_add(dllist_t* l, size_t pos, void* elem) {

    dlnode_t* insertion_node;
    dlnode_t* node;

    assert((void *)l != NULL);
    assert(pos <= l->length);
    assert(elem != NULL);

    node = malloc(sizeof(dlnode_t));
    assert((void *)node != NULL);

    node->data = malloc(l->elem_size);
    assert(node->data != NULL);

    insertion_node = get_node(l, pos);

    node->prev = insertion_node->prev;
    node->next = insertion_node;

    node->next->prev = node;
    node->prev->next = node;

    ++l->length;

    memcpy(node->data, elem, l->elem_size);
}

void dllist_dispose(dllist_t* l) {

    dlnode_t* node;
    dlnode_t* old_node;

    assert((void *)l != NULL);

    for (node = l->dummy.next; node != &l->dummy;) {

        free(node->data);

        old_node = node;
        node = node->next;

        free(old_node);
    }
}

void dllist_get(dllist_t* l, size_t pos, void* elem_out) {

    dlnode_t* node;

    assert((void *)l != NULL);
    assert(pos < l->length);
    assert(elem_out != NULL);

    node = get_node(l, pos);

    memcpy(elem_out, node->data, l->elem_size);
}

void dllist_init(dllist_t* l, size_t elem_size) {

    assert((void *)l != NULL);
    assert(elem_size > 0);

    l->length    = 0;
    l->elem_size = elem_size;

    l->dummy.next = &l->dummy;
    l->dummy.prev = &l->dummy;
    l->dummy.data = NULL;
}

void dllist_remove(dllist_t* l, size_t pos, void* elem_out) {

    dlnode_t* node;

    assert((void *)l != NULL);
    assert(pos < l->length);

    node = get_node(l, pos);

    node->prev->next = node->next;
    node->next->prev = node->prev;

    if (elem_out != NULL)
        memcpy(elem_out, node->data, l->elem_size);

    --l->length;

    free(node->data);
    free(node);
}

void dllist_set(dllist_t* l, size_t pos, void* elem, void* old_elem) {

    dlnode_t* node;

    assert((void *)l != NULL);
    assert(pos < l->length);
    assert(elem != NULL);

    node = get_node(l, pos);

    if (old_elem != NULL)
        memcpy(old_elem, node->data, l->elem_size);

    memcpy(node->data, elem, l->elem_size);
}
