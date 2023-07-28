
/**
* @file           : list.h
* @autor          : liyou
* @date           : 2022/09/16
* All rights reserved.;
* @attention
*/

#ifndef __LIST_H__
#define __LIST_H__

#include "def.h"

static inline void list_init(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new,	
				struct list_head *prev, struct list_head *next)
{
	next->prev = new;	
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add_after(struct list_head *new, 
				struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_before(struct list_head *new, 
				struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline void list_del(struct list_head *entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
    entry->next = entry;
    entry->prev = entry;
}

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#endif

