#pragma once
#include <gmp.h>
#include <stdbool.h>

/**
 * @brief An infinitely expanding list type.
 */
typedef struct {
	mpz_t *list; //!< The list of elements
	unsigned long long int size; //!< How many elements are in the list
	unsigned long long int end; //!< The last element of the list (in use)
} List;

/**
 * @brief Initialize a List.
 * @details Initialize the list and its variables, allocating memory
 * to the pointer array inside. Returns true on success and false on
 * failure.
 * @param[in] l A pointer to a List type to be initialized.
 */
void initList(List *l);

/**
 * @brief Deinitialize a List.
 * @details Release all memory that has been allocated to the list.
 * @param[in] l A pointer to a List type to be deinitialized.
 */
void deInitList(List *l);

/**
 * @brief Adds a new item to a List type.
 * @details Add item `n' at the end of a List type.
 * @param[out] l List to which the variable should be appended.
 * @param[in] n variable to be appended to the list.
 */
void addToList(List *l, mpz_t n);
