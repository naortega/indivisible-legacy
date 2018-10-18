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

#pragma once

#ifndef VERSION
#	define VERSION "version"
#endif

/* argp variables */
const char *argp_program_version = VERSION;
const char *argp_program_bug_address = "<nortega@themusicinnoise.net>";

/* argp options */
static char desc[] = "A parallelized prime number generator.";
/*static char args_doc[] = "--count=<number> [--verbose]";*/
static struct argp_option opts[] = {
	{ "count", 'c', "number", 0, "The number of primes to generate (default is 1000)", 0},
	{ "verbose", 'v', 0, 0, "Print out discovered primes", 0 },
	{ 0 }
};

struct args {
	size_t count;
	int verbose;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct args *args = state->input;
	switch(key)
	{
		case 'c':
			args->count = (size_t)atol(arg);
			break;
		case 'v':
			args->verbose = 1;
			break;
		case ARGP_KEY_ARG:
			return 0;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { opts, parse_opt, 0, desc, 0, 0, 0 };
