#include "list.h"
#include <stdlib.h>
#include <stdio.h>

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
	for(size_t i = 0; i < l->size; ++i) {
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
