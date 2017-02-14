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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>
#include <omp.h>

#include <sys/stat.h>

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
	size_t n_prime = 0;
	char *dfile = NULL;
	char *efile = NULL;

	// Parse commandline arguments
	int c;
	while((c = getopt(argc, argv, "hvqb:f:e:n:")) != -1) {
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
					return 1;
				}
				break;
			case 'f':
				dfile = optarg;
				break;
			case 'e':
				efile = optarg;
				break;
			case 'n':
				n_prime = atoi(optarg);
				if(n_prime <= 2) {
					fprintf(stderr,
							"`n' must be larger than 2 (first two primes are 2 and 3).\n");
					return 1;
				}
				break;
			default:
				printUsage(argv[0]);
				return 1;
		}
	}

	// Act based on which flags were used
	if(f_help) {
		printUsage(argv[0]);
		puts("\nArguments:");
		puts(" -h         print this help information");
		puts(" -v         print version number of program");
		puts(" -q         quiet mode");
		puts(" -b <base>  base in which to print primes between 2 and 62 (default 10)");
		puts(" -f <file>  file in/from which primes are stored and read from in raw format");
		puts(" -e <file>  export input file to plain text format");
		puts(" -n <n>     run until the 'n'th prime\n");
		return 0;
	} else if(f_version) {
		printf("Indivisible %s\n", VERSION);
		return 0;
	} else if(f_quiet && !dfile) {
		puts("Error: you cannot run in quiet mode without specifying a data file.");
		printUsage(argv[0]);
		return 0;
	}

	if(efile && !dfile) {
		puts("Error: you must have a file to export to.");
		printUsage(argv[0]);
		return 0;
	} else if(efile && dfile) {
		exportPrimes(efile, dfile, base);
		return 0;
	}

	if(!omp_get_cancellation()) {
		puts("Warning: the OpenMP cancellation environment variable (`OMP_CANCELLATION') is not enabled.");
		char in;
		while(true) {
			printf("[e]nable/[c]ontinue/[q]uit? ");
			scanf("%c", &in);
			if(in == 'e' || in == 'E') {
				putenv("OMP_CANCELLATION=true");
				execv(argv[0], argv);
			} else if(in == 'c' || in == 'C') {
				break;
			} else if(in == 'q' || in == 'Q') {
				return 0;
			}
		}
	}

	// Quit on ^C by setting `run = false'
	run = true;
	signal(SIGINT, leave);

	puts("Use Ctrl+C to exit.");

	bool newFile = true;
	if(dfile) {
		struct stat s;
		if(stat(dfile, &s) == 0) newFile = false;
	}
	// Last position added from file
	size_t startPos = 0;

	int exitCode = 0;

	// Primes we've found
	List primes;
	if(initList(&primes) == 1) {
		fprintf(stderr, "Failed to initialize primes list.\n");
		exit(1);
	}

	// The number we're going to be testing for
	mpz_t num;
	mpz_init(num);

	// Variable for sqrt of `num'
	mpz_t numRoot;
	mpz_init(numRoot);
	// Index of the prime number above and closest to `numRoot'
	size_t rootInd = 0;

	if(newFile) {
		// Add 2, a known prime to this list
		mpz_set_ui(num, 2);
		if(addToList(&primes, num) == 1) {
			fprintf(stderr, "Failed to allocate more memory for list.\n");
			exitCode = 1;
			goto releaseMemory;
		}
		// Add 3 as well to optimize the algorithm
		mpz_set_ui(num, 3);
		if(addToList(&primes, num) == 1) {
			fprintf(stderr, "Failed to allocate more memory for list.\n");
			exitCode = 1;
			goto releaseMemory;
		}
		if(!f_quiet) {
			puts("2\n3");
		}
	} else {
		// Load primes from file
		int err = inputPrimes(dfile, &primes);
		startPos = primes.end;

		if(err == 0) {
			printf("Loaded %zu primes.\n", primes.end);
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open Indivisible file `%s'.\n", dfile);
			else if(err == 2)
				fprintf(stderr, "Failed to close Indivisible file `%s'.\n", dfile);
			else if(err == 3)
				fprintf(stderr, "Failed to allocate more memory for list.\n");
			exitCode = 1;
			goto releaseMemory;
		}
		/**
		 * I set to `primes.end-1' because primes.end indicates the next new
		 * element in the list that can be used, which is also equal to the total
		 * number of elements in the list.
		 */
		mpz_set(num, primes.list[primes.end-1]);
	}

	while(run) {
		mpz_add_ui(num, num, 2);

		// Calculate the sqrt(num)
		mpz_sqrt(numRoot, num);

		while(mpz_cmp(primes.list[rootInd], numRoot) <= 0) {
			if(++rootInd > primes.end) {
				fprintf(stderr, "Error: `rootInd' surpassed `primes.end'.\n");
				exitCode = 1;
				goto releaseMemory;
			}
		}

		bool isPrime = true;
		/**
		 * Loop through primes we've found until we get to the sqrt of the
		 * number we're analyzing. Also, skip `2' since we're not testing even
		 * numbers.
		 */
		#pragma omp parallel
		{
			#pragma omp for
			for(size_t i = 1; i < rootInd; ++i) {
				if(mpz_divisible_p(num, primes.list[i])) {
					#pragma omp atomic write
					isPrime = false;
					#pragma omp cancel for
				}
				#pragma omp cancellation point for
			}
		}

		if(isPrime) {
			// `num' is a prime so we add it to the list and print it
			if(addToList(&primes, num) == 1) {
				fprintf(stderr, "Failed to allocate more memory for list.\n");
				exitCode = 1;
				goto releaseMemory;
			}
			if(!f_quiet) {
				if(!mpz_out_str(stdout, base, num))
					fprintf(stderr, "Could not print to `stdout'!\n");
				printf("\n");
			}
		}

		if(primes.end == n_prime) break;
	}


	printf("Found %zu primes.\n", primes.end);

	if(dfile) {
		int err = outputPrimes(dfile, &primes, startPos);
		if(err == 0) {
			puts("Successfully saved primes.");
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open/create file `%s'.\n", dfile);
			else if(err == 2)
				fprintf(stderr, "Failed to close file `%s'.\n", dfile);
			else if(err == 3)
				fprintf(stderr, "Failed while writing a prime to `%s'.\n", dfile);
			exitCode = 1;
			goto releaseMemory;
		}

	}

releaseMemory:
	puts("Clearing memory...");
	// Clear GMP variables
	mpz_clear(numRoot);
	mpz_clear(num);
	// Deinitialize the list
	deInitList(&primes);

	puts("Exit successful.");
	return exitCode;
}

void printUsage(char *progName) {
	printf("%s [[-f <file> [-e <file> | -q]] [-b <base>] [-n <n>] | [-h] | [-v]]\n", progName);
}

void leave() { run = false; }
