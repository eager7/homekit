/****************************************************************************
 *
 * MODULE:             list.c
 *
 * COMPONENT:          list interface
 *
 * REVISION:           $Revision: 1.0 $
 *
 * DATED:              $Date: 2016-12-02 15:13:17 +0100 (Fri, 12 Dec 2016 $
 *
 * AUTHOR:             PCT
 *
 ****************************************************************************
 *
 * Copyright panchangtao@gmail.com 2016. All rights reserved
 *
 ***************************************************************************/
#include "list.h"

void dl_list_init ( struct dl_list *list )
{
	list->next = list;
	list->prev = list;
}

/**
** head-->tail ==> head-->item-->tail
*/
void dl_list_add ( struct dl_list *list, struct dl_list *item )
{
	item->next = list->next;
	item->prev = list;
	list->next->prev = item;
	list->next = item;
}

/**
** head-->tail ==> head-->tail-->item
*/
void dl_list_add_tail ( struct dl_list *list, struct dl_list *item )
{
	dl_list_add ( list->prev, item );
}

/**
** head-->item-->tail ==> head-->tail
*/
void dl_list_del ( struct dl_list *item )
{
	item->next->prev = item->prev;
	item->prev->next = item->next;
	item->next = NULL;
	item->prev = NULL;
}

int dl_list_empty ( struct dl_list *list )
{
	return list->next == list;
}

unsigned int dl_list_len ( struct dl_list *list )
{
	struct dl_list *item;
	int count = 0;
	for ( item = list->next; item != list; item = item->next )
	{
		count++;
	}
	return count;
}


