/** !
 * Header for hash table library
 * 
 * @file hash_table/hash_table.h 
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// sync module
#include <sync/sync.h>

// log module
#include <log/log.h>

// hash cache
#include <hash_cache/hash_cache.h>
#include <hash_cache/hash.h>

// Forward declarations
struct hash_table_s;

// Type definitions
/** !
 *  @brief The type definition of a hash table struct
 */
typedef struct hash_table_s hash_table;

// Structure definitions
struct hash_table_s
{
    struct
    {
        void   **pp_data;
        size_t   count, max, length;
    } properties;
    fn_hash_cache_equality   *pfn_equality;
    fn_hash_cache_key_getter *pfn_key_get;
    fn_hash_cache_hash_index *pfn_hash_function;
};

// TODO: Allocaters
DLLEXPORT int hash_table_create ( hash_table **const pp_hash_table );

// Constructors
DLLEXPORT int hash_table_construct ( hash_table **const pp_hash_table, size_t size );

// TODO: Accessors
DLLEXPORT int hash_table_search ( hash_table *const p_hash_table, void *p_key, void **pp_value );

// TODO: Mutators
DLLEXPORT int hash_table_insert ( hash_table *const p_hash_table, void *property );

// TODO: Iterators
/** !
 * Call a function on each element of the hash table
 * 
 * @param hash_table   the hash table
 * @param pfn_function pointer to the function
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int hash_table_for_i ( const hash_table *const p_hash_table, fn_hash_cache_property_i pfn_function );

/** !
 * Call a function on each element of the hash table
 * 
 * @param hash_table   the hash table
 * @param pfn_function pointer to the function
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int hash_table_for_each ( const hash_table *const p_hash_table, fn_hash_cache_property pfn_function );

// TODO: Shallow copy
//

// TODO: Clear all items
DLLEXPORT int hash_table_clear ( hash_table *pp_hash_table, fn_hash_cache_free *pfn_free );


// TODO: Destructors
DLLEXPORT int hash_table_destroy ( hash_table **const pp_hash_table, fn_hash_cache_free *pfn_free );

