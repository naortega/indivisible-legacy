#include "files.h"
#include <stdio.h>
#include <gmp.h>
#include <assert.h>

int inputPrimes(char *file, List *list) {
	// Assert safeguards
	assert(file);
	assert(list);

	FILE *in = fopen(file, "r");
	if(!in) return 1;
	mpz_t n;
	mpz_init(n);
	while(mpz_inp_raw(n, in))
		if(addToList(list, n) == 1) return 3;

	mpz_clear(n);

	if(fclose(in)) return 2;
	return 0;
}

int outputPrimes(char *file, List *list, size_t startPos) {
	// Assert safeguards
	assert(file);
	assert(list);

	FILE *out = fopen(file, "a");
	if(!out) return 1;

	printf("Saving primes to `%s'...\n", file);
	puts("0%");
	for(size_t i = startPos; i < list->end; ++i) {
		if(!mpz_out_raw(out, list->list[i])) return 3;
		if(i == list->end / 4) puts("25%");
		else if(i == list->end / 2) puts("50%");
		else if(i == list->end * 3 / 4) puts("75%");
	}
	puts("100%");

	if(fclose(out)) return 2;
	return 0;
}

int exportPrimes(char *efile, char *dfile, int base) {
	// Assert safeguards
	assert(efile);
	assert(dfile);

	FILE *in = fopen(dfile, "r");
	FILE *out = fopen(efile, "w");
	if(!in || !out) return 1;

	printf("Exporting primes to `%s'...\n", efile);
	mpz_t n;
	mpz_init(n);
	while(mpz_inp_raw(n, in)) {
		if(!mpz_out_str(out, base, n)) return 3;
		fprintf(out, "\n");
	}

	if(fclose(in) || fclose(out)) return 2;

	return 0;
}
