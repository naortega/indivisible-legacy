#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>

#include "list.h"

#define VERSION "v0.6"

static bool run;

void printUsage(char *progName);
void leave();

int main(int argc, char *argv[]) {
	bool f_help = false,
		 f_version = false,
		 f_quiet = false;
	int base = 10;
	char *file = NULL;

	int c;
	while((c = getopt(argc, argv, "hvqb:f:")) != -1) {
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
			case 'b':
				base = atoi(optarg);
				if(base < 2 || base > 62) {
					fprintf(stderr,
							"Invalid base `%d'. Base must be between 2 and 62\n",
							base);
					exit(1);
				}
				break;
			case 'f':
				file = optarg;
				break;
			default:
				printUsage(argv[0]);
				exit(1);
		}
	}

	if(f_help) {
		printUsage(argv[0]);
		puts(" -h         print this help information");
		puts(" -v         print version number of program");
		puts(" -q         quiet mode");
		puts(" -b <base>  base in which to print primes between 2 and 62");
		puts(" -f <file>  file to save primes to");
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
		if(mpz_out_str(stdout, base, num) == 0) {
			fprintf(stderr, "Could not print to `stdout'!\n");
			goto releaseMemory;
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
			if(mpz_out_str(stdout, base, num) == 0) {
				fprintf(stderr, "Could not print to `stdout'!\n");
				goto releaseMemory;
			}
			printf("\n");
		}

nextPrime:
		// Add 2 (skip even numbers since they're all divisible by 2)
		mpz_add_ui(num, num, 2);
	} while(run);

	printf("Found %zu primes.\n", primes.end);
	// Clear GMP variables
	mpz_clear(halfNum);
	mpz_clear(num);

	if(file != NULL) {
		FILE *outFile = fopen(file, "w+");
		if(outFile == NULL) {
			fprintf(stderr, "Failed create file `%s'.\n", file);
			goto releaseMemory;
		}
		printf("Writing primes to `%s'...\n", file);
		puts("0%");
		for(size_t i = 0; i < primes.end; ++i) {
			if(mpz_out_str(outFile, base, primes.list[i]) == 0) {
				fprintf(stderr, "Error occurred while writing to file `%s'.\n", file);
				goto releaseMemory;
			}
			fprintf(outFile, "\n");
			if(i == primes.end / 4) puts("25%");
			else if(i == primes.end / 2) puts("50%");
			else if(i == primes.end * 3 / 4) puts("75%");
		}
		if(fclose(outFile) != 0) {
			fprintf(stderr, "Failed to close file `%s'.\n", file);
			goto releaseMemory;
		}
		puts("100%");
		puts("Finished writing primes.");
	}

releaseMemory:
	puts("Clearing memory...");
	// Deinitialize the list
	deInitList(&primes);

	puts("Exit successful.");
	return 0;
}

void printUsage(char *progName) {
	printf("%s [options...]\n", progName);
}

void leave() { run = false; }
