/** !
 * Include header for hash cache library
 * 
 * @file hash_cache/hash_cache.h 
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
typedef unsigned long long hash64;
typedef struct cache_s cache;
typedef struct hash_table_s hash_table;

typedef hash64 (fn_hash64)              ( const void *const k, size_t l );
typedef int    (fn_cache_equality)      ( const void *const p_a, const void *const p_b );
typedef void  *(fn_cache_key_getter)    ( const void *const p_value );
typedef void   (fn_cache_free)          ( void *p_property );
typedef int    (fn_cache_property_i)    ( void *p_property, size_t i );
typedef int    (fn_hash_table_equality) ( const void *const p_a, const void *const p_b );

// Structure definitions
struct cache_s
{
    struct
    {
        void   **pp_data;
        size_t   count, max;
    } properties;
    fn_cache_equality   *pfn_equality;
    fn_cache_key_getter *pfn_key_get;
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
 * @param pp_cache        result
 * @param size            the maximum quantity of properties the cache can fit
 * @param eviction_policy the eviction policy of the cache. < LRU | MRU | LFU | FIFO | TTL >
 * @param pfn_equality    pointer to a equality function, or 0 for default
 * @param pfn_key_get     pointer to a key getter, or 0 for key == value
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cache_construct (
    cache               **const pp_cache,
    size_t                      size,
    fn_cache_equality          *pfn_equality,
    fn_cache_key_getter        *pfn_key_get
);

// Accessors
/** !
 * Search a cache for a value using a key
 * 
 * @param p_cache   the cache
 * @param p_key     the key
 * @param pp_result return
 * 
 * @return 1 on success, 0 on error
 */


// Destructors
/** !
 * Release a cache and all its allocations
 * 
 * @param p_cache        the cache
 * @param pfn_cache_free the property deallocator if not null pointer else nothing 
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cache_destroy ( cache **const pp_cache, fn_cache_free *pfn_cache_free );

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
DLLEXPORT void hash_cache_exit ( void ) __attribute__((destructor));
