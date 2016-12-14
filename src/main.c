#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <gmp.h>

#include "list.h"

static bool run;
void leave();

int main(void) {
	puts("Indivisible v0.5\n");

	// Quit on ^C by setting `run = false'
	run = true;
	signal(SIGINT, leave);

	// Primes we've found
	List primes;
	initList(&primes);

	// The number we're going to be testing for
	mpz_t num;
	mpz_init(num);

	// Add 2, a known prime to this list
	mpz_set_ui(num, 2);
	addToList(&primes, num);
	if(mpz_out_str(stdout, 10, num) == 0) {
		fprintf(stderr, "Could not print to `stdout'!\n");
		exit(1);
	}
	printf("\n");
	mpz_add_ui(num, num, 1);

	// Variable for half `num'
	mpz_t halfNum;
	mpz_init(halfNum);

	do {
		// Calculate half of `num'
		mpz_fdiv_q_ui(halfNum, num, 2);
		// Loop through found primes
		for(size_t i = 0; i < primes.end; ++i) {
			if(mpz_cmp(primes.list[i], halfNum) > 0) break;
			// If `num' is divisible by a prime then go to the next number
			if(mpz_divisible_p(num, primes.list[i]) != 0)
				goto nextPrime;
		}

		// `num' is a prime so we add it to the list and print it
		addToList(&primes, num);
		if(mpz_out_str(stdout, 10, num) == 0) {
			fprintf(stderr, "Could not print to `stdout'!\n");
			exit(1);
		}
		printf("\n");

nextPrime:
		// Add 2 (skip even numbers since they're all divisible by 2)
		mpz_add_ui(num, num, 2);
	} while(run);

	// Clear GMP variables
	mpz_clear(halfNum);
	mpz_clear(num);
	// Deinitialize the list
	deInitList(&primes);
	return 0;
}

void leave() {
	puts("Exiting...\n");
	run = false;
}
