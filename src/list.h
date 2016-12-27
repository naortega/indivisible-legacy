/**
 * @file list.h
 * @author Deathsbreed <deathsbreed@themusicinnoise.net>
 * @brief Code responsible for List management.
 * @details Code responsible for the definition and management of the
 * List object.
 */
#pragma once
#include <gmp.h>
#include <stdbool.h>

/**
 * @brief An infinitely expanding list type.
 */
typedef struct {
	mpz_t *list; //!< The list of elements
	size_t size; //!< How many elements are in the list
	size_t end; //!< The last element of the list (in use)
} List;

/**
 * @brief Initialize a List.
 * @details Initialize the list and its variables, allocating memory
 * to the pointer array inside.
 * @param[in] l A pointer to a List type to be initialized.
 * @returns Returns 0 if successful and 1 if failed.
 */
int initList(List *restrict l);

/**
 * @brief Deinitialize a List.
 * @details Release all memory that has been allocated to the list.
 * @param[in] l A pointer to a List type to be deinitialized.
 */
void deInitList(List *restrict l);

/**
 * @brief Adds a new item to a List type.
 * @details Add item `n' at the end of a List type.
 * @param[out] l List to which the variable should be appended.
 * @param[in] n variable to be appended to the list.
 * @returns Returns 0 if successful and 1 if failed.
 */
int addToList(List *restrict l, mpz_t n);
