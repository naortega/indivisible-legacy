/*
 * Copyright (C) 2017  Ortega Froysa, Nicolás <deathsbreed@themusicinnoise.net>
 * Author: Ortega Froysa, Nicolás <deathsbreed@themusicinnoise.net>
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

#include "list.h"
#include <stdlib.h>

/**
 * This is the number of elements by which the list expands.
 */
#define BLOCK_SIZE 1024

int initList(List *restrict l) {
	l->list = malloc(sizeof(mpz_t) * BLOCK_SIZE);
	if(!l->list) return 1;
	l->size = BLOCK_SIZE;
	l->end = 0;
	return 0;
}

void deInitList(List *restrict l) {
	for(size_t i = 0; i < l->end; ++i) {
		mpz_clear(l->list[i]);
	}
	free(l->list);
}

int addToList(List *restrict l, mpz_t n) {
	if(l->end == l->size) {
		l->size += BLOCK_SIZE;
		void *tmp = realloc(l->list, sizeof(mpz_t) * l->size);
		if(!tmp) return 1;
		l->list = tmp;
	}
	mpz_init(l->list[l->end]);
	mpz_set(l->list[l->end++], n);
	return 0;
}
