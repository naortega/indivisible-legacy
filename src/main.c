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

#define VERSION "v0.7"

static bool run;

void printUsage(char *progName);
void leave();

int main(int argc, char *argv[]) {
	// Variables for argument parsing
	bool f_help = false,
		 f_version = false,
		 f_quiet = false;
	int base = 10;
	char *file = NULL;
	char *efile = NULL;

	// Parse commandline arguments
	int c;
	while((c = getopt(argc, argv, "hvqb:f:e:")) != -1) {
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
				file = optarg;
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
		puts("\nArguments:");
		puts(" -h         print this help information");
		puts(" -v         print version number of program");
		puts(" -q         quiet mode");
		puts(" -b <base>  base in which to print primes between 2 and 62 (default 10)");
		puts(" -f <file>  file in/from which primes are stored and read from in raw format");
		puts(" -e <file>  export input file to plain text format\n");
		return 0;
	} else if(f_version) {
		printf("Indivisible %s\n", VERSION);
		return 0;
	}

	// Quit on ^C by setting `run = false'
	run = true;
	signal(SIGINT, leave);

	if(efile != NULL && file == NULL) {
		fprintf(stderr, "There must be an input file to export! Use `-h' for help.\n");
		return 1;
	}
	bool newFile = true;
	if(file != NULL) {
		struct stat s;
		if(stat(file, &s) == 0) newFile = false;
	}

	int exitCode = 0;

	if(!omp_get_cancellation()) {
		puts("Warning: the OpenMP cancellation (`OMP_CANCELLATION') environment variable is not enabled.");
		char cont = '\0';
		while(true) {
			printf("Continue? (y/n) ");
			scanf("%c", &cont);
			if(cont == 'y' || cont == 'Y')
				break;
			else if(cont == 'n' || cont == 'N')
				return exitCode;
		}
	}

	// Primes we've found
	List primes;
	if(initList(&primes) == 1) {
		fprintf(stderr, "Failed to initialize primes list.\n");
		exit(1);
	}

	// The number we're going to be testing for
	mpz_t num;
	mpz_init(num);

	if(efile == NULL) puts("Use Ctrl+C to exit.");

	if(newFile) {
		// Add 2, a known prime to this list
		mpz_set_ui(num, 2);
		if(addToList(&primes, num) == 1) {
			fprintf(stderr, "Failed to allocate more memory for list.\n");
			goto releaseMemory;
		}
		// Add 3 as well to optimize the algorithm
		mpz_set_ui(num, 3);
		if(addToList(&primes, num) == 1) {
			fprintf(stderr, "Failed to allocate more memory for list.\n");
			goto releaseMemory;
		}
		if(!f_quiet) {
			puts("2\n3");
		}
	} else {
		// Load primes from file
		int err = inputPrimes(file, &primes);

		if(err == 0) {
			printf("Loaded %zu primes.\n", primes.end);
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open Indivisible file `%s'.\n", file);
			else if(err == 2)
				fprintf(stderr, "Failed to close Indivisible file `%s'.\n", file);
			else if(err == 3)
				fprintf(stderr, "Failed to allocate more memory for list.\n");
			exitCode = 1;
			goto releaseMemory;
		}
		/**
		 * Yes, I realize there's a -1 here, I don't know why but it won't
		 * work if it's not there, so don't change it unless necessary.
		 */
		mpz_set(num, primes.list[primes.end-1]);
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

	// Variable for sqrt of `privNum'
	mpz_t numRoot;
	mpz_init(numRoot);
	mpz_add_ui(num, num, 2);

	while(run) {
		// Calculate the sqrt(num)
		mpz_sqrt(numRoot, num);

		bool isPrime = true;
		/**
		 * Loop through primes we've found until we get to the sqrt of the
		 * number we're analyzing. Also, skip `2' since we're not testing even
		 * numbers.
		 */
		#pragma omp parallel
		{
			#pragma omp for
			for(size_t i = 1; i < primes.end; ++i) {
				if(mpz_cmp(primes.list[i], numRoot) >= 0) {
					#pragma omp cancel for
				} else if(mpz_divisible_p(num, primes.list[i])) {
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
				run = false;
			}
			if(!f_quiet) {
				if(mpz_out_str(stdout, base, num) == 0)
					fprintf(stderr, "Could not print to `stdout'!\n");
				printf("\n");
			}
		}

		mpz_add_ui(num, num, 2);
	}

	mpz_clear(numRoot);

	printf("Found %zu primes.\n", primes.end);

	if(file != NULL) {
		int err = outputPrimes(file, &primes);
		if(err == 0) {
			puts("Successfully saved primes.");
		} else {
			if(err == 1)
				fprintf(stderr, "Failed to open/create file `%s'.\n", file);
			else if(err == 2)
				fprintf(stderr, "Failed to close file `%s'.\n", file);
			else if(err == 3)
				fprintf(stderr, "Failed while writing a prime to `%s'.\n", file);
			exitCode = 1;
			goto releaseMemory;
		}

	}

releaseMemory:
	puts("Clearing memory...");
	// Clear GMP variables
	mpz_clear(num);
	// Deinitialize the list
	deInitList(&primes);

	puts("Exit successful.");
	return exitCode;
}

void printUsage(char *progName) {
	printf("%s [-f <file>] [-q] [-b <base>]\n", progName);
	printf("%s -f <file> -e <file>\n", progName);
}

void leave() { run = false; }
