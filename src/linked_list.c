/*
 * Copyright (C) 2018  Ortega Froysa, Nicolás <nortega@themusicinnoise.net>
 * Author: Ortega Froysa, Nicolás <nortega@themusicinnoise.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linked_list.h"

#include <stdlib.h>

void llist_init(struct llist *list) {
	list->size = 0;
	list->first = NULL;
	list->last = NULL;
}

int llist_add(struct llist *list, mpz_t num) {
	struct llist_item *item =
		malloc(sizeof(struct llist_item));
	if(!item)
		return 0;

	mpz_init(item->num);
	mpz_set(item->num, num);

	if(!(list->first))
	{
		list->first = item;
		list->last = item;
	}
	else
	{
		list->last->next = item;
		list->last = item;
	}
	list->size++;

	return 1;
}

void llist_deinit(struct llist *list) {
	struct llist_item *i = list->first;

	while(i)
	{
		mpz_clear(i->num);
		struct llist_item *next = i->next;
		free(i);
		i = next;
	}

	list->size = 0;
}
