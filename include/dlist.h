#ifndef DLIST_H
#define DLIST_H

#include <stddef.h>

struct dl_list {
    struct dl_list* next;
    struct dl_list* prev;
};

// create new dl_list head
#define DL_LIST_HEAD(name) \
    struct dl_list name = { &(name), &(name) }

static inline void list_init(struct dl_list* head) {
    head->prev = head->next = head;
}

static inline void list_add(struct dl_list* new, struct dl_list* head) {
    head->next->prev = new;
    new->next = head->next;
    new->prev = head;
    head->next = new;
}

static inline void list_add_tail(struct dl_list* new, struct dl_list* head) {
    head->prev->next = new;
    new->prev = head->prev;
    new->next = head;
    head->prev = new;
}

static inline void list_del(struct dl_list* node) {
    struct dl_list* prev = node->prev;
    struct dl_list* next = node->next;

    prev->next = next;
    next->prev = prev;
}

// retrieve the container structure that contains the given `list_head`
#define list_entry(ptr, type, member) \
    ((type *) ((char *) (ptr) - offsetof(type, member)))

// retrieve first element in list as its container structure
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

// iterate over all nodes in the list
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

// safer version of `list_for_each`, allowing for modifications while iterating
#define list_for_each_safe(pos, p, head)    \
    for (pos = (head)->next, p = pos->next; \
         pos != (head);                     \
         pos = p, p = pos->next)

static inline int list_empty(struct dl_list* head) {
    return head->next == head;
}

#endif
