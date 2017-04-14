#ifndef __LIST_H__
#define __LIST_H__

/*
 * Simple linked-list routines, based on functions of the same name from the Linux kernel.
 */

struct list_head {
        struct list_head *next, *prev;
};

static inline void INIT_LIST_HEAD(struct list_head *list)
{
        list->next = list;
        list->prev = list;
}

static inline int list_empty(const struct list_head *head)
{
        return head->next == head;
}

static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
        __list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        entry->next = entry->prev = NULL;
}

#define list_for_each(pos, head) \
         for (pos = (head)->next; pos != (head); pos = pos->next)

#endif
