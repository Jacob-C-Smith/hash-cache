/** !
 * Include header for hash library
 * 
 * @file hash/hash.h 
 * 
 * @author Jacob Smith 
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stddef.h>

// sync module
#include <sync/sync.h>

// log module
#include <log/log.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Memory management macro
#ifndef HASH_CACHE_REALLOC
#define HASH_CACHE_REALLOC(p, sz) realloc(p,sz)
#endif

// Structure declarations
struct cache_s;
struct hash_table_s;

// Type definitions
typedef struct cache_s cache;
typedef struct hash_table_s hash_table;
typedef unsigned long long hash64; 
typedef hash64 (fn_hash64)( const void *const k, size_t l );

// Structure definitions
struct cache_s
{
    int (*pfn_equality)(const void *const p_a, const void *const p_b);
    struct
    {
        void   **pp_data;
        size_t   count, max;
    } properties;
};

struct hash_table_s
{
    struct
    {
        size_t   max;
        void   **pp_data;
    } entries;

    size_t key_size;
    // TODO: Equality function
};

// Function declarations 

// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
DLLEXPORT void hash_cache_init ( void ) __attribute__((constructor));

// Hashing
/** !
 * Compute a 64-bit hash using the Fowler–Noll–Vo hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
DLLEXPORT hash64 hash_fnv64 ( const void *const k, size_t l );

/** !
 * Compute a 64-bit hash using the MurMur hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
DLLEXPORT hash64 hash_mmh64 ( const void *const k, size_t l );

/** !
 * Compute a 64-bit hash using the xxHash hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
DLLEXPORT hash64 hash_xxh64 ( const void *const k, size_t l );

/** !
 * Compute a 64-bit hash using the CRC hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
DLLEXPORT hash64 hash_crc64 ( const void *const k, size_t l );

// Allocators
/** !
 * Allocate memory for a cache
 * 
 * @param pp_cache result
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cache_create ( cache **const pp_cache );

/** !
 * Allocate memory for a hash table 
 * 
 * @param pp_hash_table result
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int hash_table_create ( hash_table **const pp_hash_table );

// Constructors
/** !
 * Construct a cache
 * 
 * @param pp_cache result
 * @param size     the maximum quantity of properties the cache can fit
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cache_construct ( cache **const pp_cache, size_t size );


// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
DLLEXPORT void hash_cache_exit ( void ) __attribute__((destructor));
