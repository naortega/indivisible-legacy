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

#include "files.h"
#include <stdio.h>
#include <gmp.h>
#include <assert.h>

int inputPrimes(char *file, List *list) {
	// Assert safeguards
	assert(file);
	assert(list);

	printf("Loading primes from `%s'...\n", file);
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
		if(i - startPos == (list->end - startPos) / 4) puts("25%");
		else if(i - startPos == (list->end - startPos) / 2) puts("50%");
		else if(i - startPos == (list->end - startPos) * 3 / 4) puts("75%");
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
