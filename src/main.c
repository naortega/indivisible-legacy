#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>

#include "list.h"
#include "files.h"

#define VERSION "v1.0"

static bool run;

void printUsage(char *progName);
void leave();

int main(int argc, char *argv[]) {
	// Variables for argument parsing
	bool f_help = false,
		 f_version = false,
		 f_quiet = false;
	int base = 10;
	char *ofile = NULL;
	char *ifile = NULL;
	char *efile = NULL;

	// Parse commandline arguments
	int c;
	while((c = getopt(argc, argv, "hvqb:o:i:e:")) != -1) {
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
							"Invalid base `%d'. Base must be between 2 and 62.\n",
							base);
					exit(1);
				}
				break;
			case 'o':
				ofile = optarg;
				break;
			case 'i':
				ifile = optarg;
				break;
			case 'e':
				efile = optarg;
				break;
			default:
				printUsage(argv[0]);
				return 1;
		}
	}

	// Act based on which flags were used
	if(f_help) {
		printUsage(argv[0]);
		puts(" -h         print this help information");
		puts(" -v         print version number of program");
		puts(" -q         quiet mode");
		puts(" -b <base>  base in which to print primes between 2 and 62 (default 10)");
		puts(" -o <file>  file to save primes to");
		puts(" -i <file>  file to read primes from a previous session");
		puts(" -e <file>  export input file to plain text format");
		return 0;
	} else if(f_version) {
		printf("Indivisible %s\n", VERSION);
		return 0;
	}

	// Quit on ^C by setting `run = false'
	run = true;
	signal(SIGINT, leave);

	if(efile != NULL && ifile == NULL) {
		fprintf(stderr, "There must be an input file to export! Use `-h' for help.\n");
		return 1;
	}

	int exitCode = 0;

	// Primes we've found
	List primes;
	initList(&primes);

	// The number we're going to be testing for
	mpz_t num;
	mpz_init(num);

	// Variable for half `num'
	mpz_t halfNum;
	mpz_init(halfNum);

	if(efile == NULL) puts("Use Ctrl+C to exit.");

	if(ifile == NULL) {
		// Add 2, a known prime to this list
		mpz_set_ui(num, 2);
		addToList(&primes, num);
		if(!f_quiet) {
			if(mpz_out_str(stdout, base, num) == 0) {
				fprintf(stderr, "Could not print to `stdout'!\n");
				exitCode = 1;
				goto releaseMemory;
			}
			printf("\n");
		}
		mpz_add_ui(num, num, 1);
	} else {
		// Load primes from file
		int err = inputPrimes(ifile, &primes);

		if(err == 0) {
			printf("Loaded %zu primes.\n", primes.end);
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open Indivisible file `%s'.\n", ifile);
			else if(err == 2)
				fprintf(stderr, "Failed to close Indivisible file `%s'.\n", ifile);
			exitCode = 1;
			goto releaseMemory;
		}
		mpz_set(num, primes.list[primes.end]);
	}

	if(efile != NULL) {
		int err = exportPrimes(efile, &primes, base);

		if(err == 0) {
			puts("Successfully exported primes.");
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open/create plain text file `%s'.\n", efile);
			else if(err == 2)
				fprintf(stderr, "Failed to close plain text file `%s'.\n", efile);
			else if(err == 3)
				fprintf(stderr, "Failed to write prime to plain text file `%s'.\n", efile);
			exitCode = 1;
		}
		goto releaseMemory;
	}

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
				exitCode = 1;
				goto releaseMemory;
			}
			printf("\n");
		}

nextPrime:
		// Add 2 (skip even numbers since they're all divisible by 2)
		mpz_add_ui(num, num, 2);
	} while(run);

	printf("Found %zu primes.\n", primes.end);

	if(ofile != NULL) {
		int err = outputPrimes(ofile, &primes);
		if(err == 0) {
			puts("Successfully saved primes.");
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open/create file `%s'.\n", ofile);
			else if(err == 2)
				fprintf(stderr, "Failed to close file `%s'.\n", ofile);
			else if(err == 3)
				fprintf(stderr, "Failed while writing a prime to `%s'.\n", ofile);
			exitCode = 1;
			goto releaseMemory;
		}

	}

releaseMemory:
	puts("Clearing memory...");
	// Clear GMP variables
	mpz_clear(halfNum);
	mpz_clear(num);
	// Deinitialize the list
	deInitList(&primes);

	puts("Exit successful.");
	return exitCode;
}

void printUsage(char *progName) {
	printf("%s [OPTIONS]\n", progName);
}

void leave() { run = false; }
