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

#include "prime_test.h"

int test_prime(const mpz_t p) {
	mpz_t psqrt;
	mpz_t aux;
	mpz_inits(psqrt, aux, NULL);
	mpz_sqrt(psqrt, p);
	int is_prime = 1;
	for(mpz_set_ui(aux, 5); mpz_cmp(aux, psqrt) <= 0; mpz_add_ui(aux, aux, 2))
	{
		if(mpz_divisible_p(p, aux))
		{
			is_prime = 0;
			break;
		}
	}

	mpz_clears(psqrt, aux, NULL);

	return is_prime;
}
