/*
 * Copyright (C) 2019  Ortega Froysa, Nicolás <nicolas@ortegas.org>
 * Author: Ortega Froysa, Nicolás <nicolas@ortegas.org>
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

#include "llist.h"

#include <stdlib.h>
#include <stdio.h>

void llist_init(struct llist *list) {
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
}

void llist_deinit(struct llist *list) {
	struct llist_item *aux = list->first;
	while(aux)
	{
		mpz_clear(aux->n);
		struct llist_item *tmp = aux;
		aux = aux->next;
		free(tmp);
	}
}

int llist_insert(struct llist *list, mpz_t n) {
	struct llist_item *aux =
		malloc(sizeof(struct llist_item));
	if(!aux)
	{
		fprintf(stderr, "ERROR llist_insert(): failed to allocate memory");
		return 0;
	}
	aux->prev = list->last;
	aux->next = NULL;
	// TODO: check time complexity of these GMP functions
	mpz_init(aux->n);
	mpz_set(aux->n, n);

	if(!list->first)
		list->first = aux;
	list->last = aux;
	list->size++;

	return 1;
}
