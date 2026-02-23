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
#include <stdio.h>
#include "llist.h"
#include "prime_test.h"

#include <signal.h>
#include <gmp.h>

int run = 1;

void quit_signal(int signum) {
	printf("Received signal %d. Ending process...\n", signum);
	run = 0;
}

int main() {
	printf("%s v%s\n", APP_NAME, VERSION);

	signal(SIGINT, quit_signal);

	mpz_t test_base; // number used to get tested numbers
	mpz_t p0, p1; // numbers to be tested for prime-ness.

	mpz_inits(test_base, p0, p1, NULL);
	mpz_set_ui(test_base, 6); // test_base = 6*k

	puts("2");
	puts("3");

	while(run)
	{
		mpz_sub_ui(p0, test_base, 1); // p0 = test_base - 1
		mpz_add_ui(p1, test_base, 1); // p0 = test_base + 1

		if(test_prime(p0))
		{
			mpz_out_str(stdout, 10, p0);
			printf("\n");
		}
		if(test_prime(p1))
		{
			mpz_out_str(stdout, 10, p1);
			printf("\n");
		}

		mpz_add_ui(test_base, test_base, 6); // k += 1
	}

	mpz_clears(test_base, p0, p1, NULL);

	return 0;
}
