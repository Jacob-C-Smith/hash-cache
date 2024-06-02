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
        h ^= (unsigned long long)((char *)k)[i];

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
                    printf("[hash cache] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
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
        unsigned long long n = *data++;

        n *= m;
        n ^= n >> r;
        n *= m;

        h ^= n;
        h *= m;
    }

    d2 = (unsigned char *)data;

    switch ( l & 7 )
    {
        case 7: h ^= (unsigned long long)(d2[6]) << 48; __attribute__((fallthrough));
        case 6: h ^= (unsigned long long)(d2[5]) << 40; __attribute__((fallthrough));
        case 5: h ^= (unsigned long long)(d2[4]) << 32; __attribute__((fallthrough));
        case 4: h ^= (unsigned long long)(d2[3]) << 24; __attribute__((fallthrough));
        case 3: h ^= (unsigned long long)(d2[2]) << 16; __attribute__((fallthrough));
        case 2: h ^= (unsigned long long)(d2[1]) << 8;  __attribute__((fallthrough));
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
                    printf("[hash cache] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_create ( cache **const pp_cache )
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;

    // Initialized data
    cache *p_cache = HASH_CACHE_REALLOC(0, sizeof(cache));

    // Error check
    if ( p_cache == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_cache, 0, sizeof(cache));

    // Return a pointer to the caller
    *pp_cache = p_cache;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_create ( hash_table **const pp_hash_table )
{
    
    // Argument errors
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Initialized data
    hash_table *p_hash_table = HASH_CACHE_REALLOC(0, sizeof(hash_table));

    // Error check
    if ( p_hash_table == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_hash_table, 0, sizeof(hash_table));

    // Return a pointer to the caller
    *pp_hash_table = p_hash_table;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_construct ( cache **const pp_cache, size_t size )
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;
    if ( size     ==          0 ) goto invalid_size;



    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
            invalid_size:
                #ifndef NDEBUG
                    log_error("[hash cache] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }
    }
}

/*
int hash_table_construct ( hash_table **const pp_hash_table, size_t size )
{

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;
    if ( size          ==          0 ) goto invalid_size;



    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
            invalid_size:
                #ifndef NDEBUG
                    log_error("[hash cache] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }
    }
}

int cache_get ( const cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;

    // Search the cache
    for (size_t i = 0; i < p_cache->entries; i++)
    {
        
        // Compare
        if ( p_cache->pfn_equals(p_cache->pfn_key(p_cache->data[i]), p_key ) )
        {

            // Move the property to the front of the cache 

            // Return the value to the caller 
            *pp_result = p_cache->data[i];

            // Success
            return 1;
        }
    }

    // Miss
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_search ( const hash_table *const p_hash_table, const void *const p_key )
{

    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    if ( p_hash_table == (void *) 0 ) goto no_key;

    

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;


        }
    }
}

int cache_add ( cache *const p_cache, const void *const p_key, const void *const p_value )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;
    if ( p_value == (void *) 0 ) goto no_value;

    // If the cache is full ...
    if ( p_cache->entries == p_cache->max )
    {
        
        // ... remove the last object in the cache ...
        // ... and decrement the quantity of entries
    }

    // Add the object to the cache

    // Increment the quantity of entries

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_insert ( const hash_table *const p_hash_table, const void *const p_key )
{

    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    if ( p_hash_table == (void *) 0 ) goto no_key;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;


        }
    }
}

int cache_remove ( cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;

    // Initialized data
    size_t i = 0;
    void *p_value = (void *) 0;

    // Search the cache
    for (; i < p_cache->entries; i++)
        
        // Compare
        if ( p_cache->pfn_equals(p_cache->pfn_key(p_cache->data[i]), p_key ) )
        {

            // Move the property to the front of the cache 

            // Return the value to the caller 
            p_value = p_cache->data[i];
        }

    // If the cache hit
    if ( p_value )
    {

        // Remove the element from the cache

        // Decrement the property count

        // Return a pointer to the caller
        if ( *pp_result ) *pp_result = p_value;

        // Success
        return 1;
    }

    // Miss
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_clear ( cache *p_cache )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;

    // Clear the property counter
    p_cache->entries = 0;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_destroy ( cache **const pp_cache, fn_cache_free *pfn_cache_free )
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache; 

}

int hash_table_destroy ( hash_table **const pp_hash_table, fn_hash_cache_free *pfn_free )
{

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table; 

    

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
*/

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
