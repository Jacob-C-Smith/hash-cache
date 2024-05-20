/** !
 * Hashing functions and caching abstractions
 * 
 * @file hash_cache.c 
 * 
 * @author Jacob Smith
 */

// Header
#include <hash_cache/hash_cache.h>

// Data
static bool initialized = false;

void hash_cache_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

hash64 hash_fnv64 ( const void* const k, size_t l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Constant data
    const unsigned long long p = 0x100000001B3;

    // Initialized data
    unsigned long long h = 0xc6a4a7935bd1e995;
    
    // Compute the hash
    for (size_t i = 0; i < l; i++)
    {

        // XOR the eight least significant bits of the hash
        h ^= ((char *)k)[i];

        // Multiply the hash by the prime
        h *= p;
    }
    
    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[crypto] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

hash64 hash_mmh64 ( const void* const k, size_t l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Constant data
    const unsigned long long m = 0xc6a4a7935bd1e995;
    const int                r = 47;

    // Initialized data
    unsigned long long   h    = 0x41C64E6D ^ (l * m);
    unsigned long long  *data = (unsigned long long*)k;
    unsigned long long  *end  = (l >> 3) + data;
    unsigned char       *d2   = 0;

    // Compute the hash
    while ( data != end )
    {
        unsigned long long k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    d2 = (unsigned char *)data;

    switch ( l & 7 )
    {
        case 7: h ^= (unsigned long long)(d2[6]) << 48;
        case 6: h ^= (unsigned long long)(d2[5]) << 40;
        case 5: h ^= (unsigned long long)(d2[4]) << 32;
        case 4: h ^= (unsigned long long)(d2[3]) << 24;
        case 3: h ^= (unsigned long long)(d2[2]) << 16;
        case 2: h ^= (unsigned long long)(d2[1]) << 8;
        case 1: h ^= (unsigned long long)(d2[0]);
            h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[crypto] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void hash_cache_exit ( void )
{

    // State check
    if ( initialized == false ) return;

    // Clean up the log library
    log_exit();

    // Clean up the sync library
    sync_exit();

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
