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

// Type definitions
typedef unsigned long long hash64; 
typedef hash64 (fn_hash64)( const void *const k, size_t l );

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

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
DLLEXPORT void hash_cache_exit ( void ) __attribute__((destructor));
