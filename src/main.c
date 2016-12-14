#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <getopt.h>
#include <gmp.h>

#include "list.h"

#define VERSION "v0.5"

static bool run;

void printUsage(char *progName);
void leave();

int main(int argc, char *argv[]) {
	bool f_help = false, f_version = false, f_quiet = false;

	int c;
	while((c = getopt(argc, argv, "hvq")) != -1) {
		switch(c) {
			case 'h':
				f_help = true;
				break;
			case 'v':
				f_version = true;
				break;
			case 'q':
				f_quiet = true;
				break;
			default:
				printUsage(argv[0]);
				exit(1);
		}
	}

	if(f_help) {
		printUsage(argv[0]);
		puts(" -h     print this help information");
		puts(" -v     print version number of program");
		puts(" -q     quiet mode");
		return 0;
	} else if(f_version) {
		printf("Indivisible %s\n", VERSION);
		return 0;
	}

	// Quit on ^C by setting `run = false'
	run = true;
	signal(SIGINT, leave);

	if(f_quiet) {
		puts("Use Ctrl+C (SIGINT) to exit.");
	}

	// Primes we've found
	List primes;
	initList(&primes);

	// The number we're going to be testing for
	mpz_t num;
	mpz_init(num);

	// Add 2, a known prime to this list
	mpz_set_ui(num, 2);
	addToList(&primes, num);
	if(!f_quiet) {
		if(mpz_out_str(stdout, 10, num) == 0) {
			fprintf(stderr, "Could not print to `stdout'!\n");
			exit(1);
		}
		printf("\n");
	}
	mpz_add_ui(num, num, 1);

	// Variable for half `num'
	mpz_t halfNum;
	mpz_init(halfNum);

	do {
		// Calculate half of `num'
		mpz_fdiv_q_ui(halfNum, num, 2);
		/**
		 * Loop through primes we've found until we get to half of the number
		 * we're analyzing
		 */
		for(size_t i = 0; mpz_cmp(primes.list[i], halfNum) < 0; ++i) {
			// If `num' is divisible by a prime then go to the next number
			if(mpz_divisible_p(num, primes.list[i]) != 0)
				goto nextPrime;
		}

		// `num' is a prime so we add it to the list and print it
		addToList(&primes, num);
		if(!f_quiet) {
			if(mpz_out_str(stdout, 10, num) == 0) {
				fprintf(stderr, "Could not print to `stdout'!\n");
				exit(1);
			}
			printf("\n");
		}

nextPrime:
		// Add 2 (skip even numbers since they're all divisible by 2)
		mpz_add_ui(num, num, 2);
	} while(run);

	printf("Found %zu primes.\n", primes.end);
	puts("Clearing memory...");
	// Clear GMP variables
	mpz_clear(halfNum);
	mpz_clear(num);
	// Deinitialize the list
	deInitList(&primes);

	puts("Exit successful.");
	return 0;
}

void printUsage(char *progName) {
	printf("%s [-v | -h | -q]\n", progName);
}

void leave() { run = false; }
