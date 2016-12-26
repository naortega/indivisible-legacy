#include "files.h"
#include <stdio.h>
#include <gmp.h>

int inputPrimes(char *file, List *list) {
	FILE *pFile = fopen(file, "r");
	if(pFile == NULL) return 1;
	mpz_t n;
	mpz_init(n);
	while(mpz_inp_raw(n, pFile) != 0) addToList(list, n);
	if(fclose(pFile) != 0) return 2;
	return 0;
}

int outputPrimes(char *file, List *list) {
	FILE *oFile = fopen(file, "w");
	if(oFile == NULL) return 1;
	printf("Saving primes to `%s'...\n", file);
	puts("0%");
	for(size_t i = 0; i < list->end; ++i) {
		if(mpz_out_raw(oFile, list->list[i]) == 0) return 3;
		if(i == list->end / 4) puts("25%");
		else if(i == list->end / 2) puts("50%");
		else if(i == list->end * 3 / 4) puts("75%");
	}
	puts("100%");
	if(fclose(oFile) != 0) return 2;
	return 0;
}

int exportPrimes(char *file, List *list, int base) {
	FILE *eFile = fopen(file, "w");
	if(eFile == NULL) return 1;
	printf("Exporting primes to `%s'...\n", file);
	puts("0%");
	for(size_t i = 0; i < list->end; ++i) {
		if(mpz_out_str(eFile, list->list[i], base) == 0) return 3;
		if(i == list->end / 4) puts("25%");
		else if(i == list->end / 2) puts("50%");
		else if(i == list->end * 3 / 4) puts("75%");
	}
	puts("100%");
	if(fclose(eFile) != 0) return 2;
	puts("Finished exporting primes.");
	return 0;
}