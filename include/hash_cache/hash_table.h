/** !
 * Header for hash table library
 * 
 * @file hash_table/hash_table.h 
 * @date Feb 18, 2024
 * @author Jacob Smith
 */

// Include guard
#pragma once

#undef NDEBUG

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// sync submodule
#include <sync/sync.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Memory management macro
#ifndef HASH_TABLE_REALLOC
#define HASH_TABLE_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct hash_table_s;

// Type definitions
/** !
 *  @brief The type definition of a hash table struct
 */
typedef struct hash_table_s hash_table;

// TODO: Allocaters
//

// TODO: Constructors
//

// TODO: Accessors
//

// TODO: Mutators
//

// TODO: Iterators
//

// TODO: Shallow copy
//

// TODO: Clear all items
//

// TODO: Destructors
//
