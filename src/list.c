#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * This is the number of elements by which the list expands.
 * WARNING: Always use doubles for this number (2^X)
 */
#define BLOCK_SIZE 1024

void initList(List *restrict l) {
	l->list = malloc(sizeof(mpz_t) * BLOCK_SIZE);
	if(!l->list) {
		fprintf(stderr, "Failed to allocate memory to list!\n");
		exit(1);
	}
	l->size = BLOCK_SIZE;
	l->end = 0;
}

void deInitList(List *restrict l) {
	for(ulli i = 0; i < l->size; ++i) {
		mpz_clear(l->list[i]);
	}
	free(l->list);
}

void addToList(List *l, mpz_t n) {
	if(l->end == l->size) {
		l->size += BLOCK_SIZE;
		if(l->size == 0) {
			fprintf(stderr, "`l->size' has overflowed!\n");
			exit(1);
		}
		void *tmp = realloc(l->list, sizeof(mpz_t) * l->size);
		if(!tmp) {
			fprintf(stderr, "Failed to allocate more memory to list!\n");
			exit(1);
		}
		l->list = (mpz_t*)tmp;
	}
	mpz_init(l->list[l->end]);
	mpz_set(l->list[l->end++], n);
}
