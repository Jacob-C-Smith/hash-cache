/** !
 * Header for cache
 * 
 * @file hash_cache/cache.h 
 *
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

// sync module
#include <sync/sync.h>

// hash cache
#include <hash_cache/hash.h>

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
struct cache_s;

// Type definitions
/** !
 *  @brief The type definition of a cache struct
 */
typedef struct cache_s cache;

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
