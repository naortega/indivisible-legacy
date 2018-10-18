/*
 * Copyright (C) 2018  Ortega Froysa, Nicolás <nortega@themusicinnoise.net>
 * Author: Ortega Froysa, Nicolás <nortega@themusicinnoise.net>
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
#include <argp.h>
#include <gmp.h>

#include "global.h"
#include "linked_list.h"

int main(int argc, char *argv[]) {
	struct args args = { 1000, 0 };

	argp_parse(&argp, argc, argv, 0, 0, &args);

	if(args.count == 0)
	{
		fprintf(stderr, "ERROR: count must be larger than 0.\n");
		return 1;
	}

	struct llist prime_list;
	llist_init(&prime_list);

	{
		// initialize the list with 2
		mpz_t tmp;
		mpz_init(tmp);
		mpz_set_ui(tmp, 2);
		llist_add(&prime_list, tmp);
		mpz_clear(tmp);
		if(args.verbose)
			puts("2");
	}

	mpz_t aux;
	mpz_init(aux);
	mpz_set_ui(aux, 3);
	while(prime_list.size < args.count)
	{
		struct llist_item *item = prime_list.first;
		int is_prime = 1;
		while(item)
		{
			if(mpz_divisible_p(aux, item->num))
			{
				is_prime = 0;
				break;
			}
			item = item->next;
		}
		if(is_prime)
		{
			llist_add(&prime_list, aux);
			if(args.verbose)
			{
				mpz_out_str(stdout, 10, aux);
				printf("\n");
			}
		}
		mpz_add_ui(aux, aux, 2);
	}

	printf("The %zu prime is ", prime_list.size);
	mpz_out_str(stdout, 10, prime_list.last->num);

	llist_deinit(&prime_list);
	return 0;
}
