/**
 * @file files.h
 * @author Deathsbreed <deathsbreed@themusicinnoise.net>
 * @brief Functions to deal with file I/O of primes.
 * @details Functions that input, output, and export primes from/to files.
 */
#pragma once
#include "list.h"

/**
 * @brief Load primes from an Indivisible file into a List.
 * @param file File to input primes from.
 * @param list List to load primes into.
 * @returns If 0 then load was successful, if 1 then failed to open,
 * if 2 failed to close, if 3 failed to allocate new memory to List
 * (see `addToList()')
 */
int inputPrimes(char *file, List *list);

/**
 * @brief Output primes from a List into an Indivisible file.
 * @param file File to output primes to.
 * @param list List to read primes from.
 * @param startPos The position in the List of primes at which to append
 * to the file.
 * @returns If 0 then load was successful, if 1 then failed to open,
 * if 2 failed to close, if 3 failed when writing.
 */
int outputPrimes(char *file, List *list, size_t startPos);

/**
 * @brief Export primes from a List to a plain text file.
 * @param efile File to export primes as plain text to.
 * @param dfile File to read primes from.
 * @param base The base in which the primes will be written.
 * @returns If 0 then load was successful, if 1 then failed to open,
 * if 2 failed to close, if 3 failed when writing.
 */
int exportPrimes(char *efile, char *dfile, int base);
