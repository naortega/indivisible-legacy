/*
 * Copyright (C) 2019  Ortega Froysa, Nicolás <nicolas@ortegas.org>
 * Author: Ortega Froysa, Nicolás <nicolas@ortegas.org>
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

#include "globals.h"
#include "llist.h"

#include <signal.h>
#include <stdio.h>
#include <gmp.h>

int run = 1;

void quit_signal(int signum) {
	printf("Received signal %d. Ending process...\n", signum);
	run = 0;
}

int main() {
	printf("%s v%s\n", APP_NAME, VERSION);

	signal(SIGINT, quit_signal);

	mpz_t tnum; // number to be tested for prime-ness
	mpz_t tsqrt; // square root of tnum
	mpz_t aux; // auxilary number to test against tnum
	mpz_t r; // remainder from modulus operation

	mpz_inits(tnum, tsqrt, aux, r, NULL);
	mpz_set_ui(tnum, 3);
	mpz_sqrt(tsqrt, tnum);

	while(run)
	{
		mpz_set_ui(aux, 3);
		int is_prime = 1;
		while(mpz_cmp(aux, tsqrt) <= 0)
		{
			mpz_mod(r, tnum, aux);
			if(mpz_cmp_ui(r, 0) == 0)
				is_prime = 0;
			mpz_add_ui(aux, aux, 2);
		}

		if(is_prime)
		{
			mpz_out_str(stdout, 10, tnum);
			printf("\n");
		}

		mpz_add_ui(tnum, tnum, 2);
		mpz_sqrt(tsqrt, tnum);
	}

	mpz_clears(tnum, tsqrt, aux, r, NULL);

	return 0;
}
