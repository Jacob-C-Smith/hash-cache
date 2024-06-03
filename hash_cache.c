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
static unsigned long long crc64_table[256] = { 0 };

// Function declarations
/** !
 * Bitwise left rotate an unsigned 64 bit value by N bits
 * 
 * @param x the unsigned 64 bit value
 * @param r N bits
 * 
 * @return rotated x
 */
unsigned long long rotl64 ( unsigned long long x, signed char r );

/** !
 * Default comparator
 * 
 * @param p_a A
 * @param p_b B
 * 
 * @return 0 if A == B else 1
 */
int cache_equals ( const void *const p_a, const void *p_b );

void hash_cache_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Initialize the CRC64 table
    for (unsigned long long i = 0; i < 256; i++)
    {
        
        // Initialized data
        unsigned long long crc = i;

        for (size_t j = 0; j < 8; j++)

            if ( crc & 1 ) crc = (crc >> 1) ^ 0xC96C5795D7870F42;
            else           crc >>= 1;
        
        // Store the value
        crc64_table[i] = crc;        
    }
    
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

hash64 hash_xxh64 ( const void *const k, size_t l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Initialized data
    hash64 h = 0x27D4EB2F165667C5;
    const unsigned char* p = (const unsigned char*)k;
    const unsigned char* const end = p + l;

    // Long input
    if ( l >= 32 )
    {
        
        // Initialized data
        const unsigned char* const limit = end - 32;
        unsigned long long v1 = (0x9E3779B185EBCA87 + 0xC2B2AE3D27D4EB4F);
        unsigned long long v2 = 0xC2B2AE3D27D4EB4F;
        unsigned long long v3 = 0;
        unsigned long long v4 = 0x9E3779B185EBCA87;

        // Compute the
        do
        {
            // Initialized data
            unsigned long long k1 = *(const unsigned long long*)(p);
            unsigned long long k2 = *(const unsigned long long*)(p + 8);
            unsigned long long k3 = *(const unsigned long long*)(p + 16);
            unsigned long long k4 = *(const unsigned long long*)(p + 24);

            v1 += k1 * 0xC2B2AE3D27D4EB4F;
            v1 = rotl64(v1, 31);
            v1 *= 0x9E3779B185EBCA87;

            v2 += k2 * 0xC2B2AE3D27D4EB4F;
            v2 = rotl64(v2, 31);
            v2 *= 0x9E3779B185EBCA87;

            v3 += k3 * 0xC2B2AE3D27D4EB4F;
            v3 = rotl64(v3, 31);
            v3 *= 0x9E3779B185EBCA87;

            v4 += k4 * 0xC2B2AE3D27D4EB4F;
            v4 = rotl64(v4, 31);
            v4 *= 0x9E3779B185EBCA87;

            p += 32;
        }
        
        // Continuation condition
        while ( p <= limit );

        h = rotl64(v1, 1) + rotl64(v2, 7) + rotl64(v3, 12) + rotl64(v4, 18);
        h = (h ^ (rotl64(v1 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87)) * 0x9E3779B185EBCA87 + 0x85EBCA77C2B2AE63;
        h += rotl64(v2 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87;
        h = (h ^ (rotl64(v3 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87)) * 0x9E3779B185EBCA87 + 0x85EBCA77C2B2AE63;
        h += rotl64(v4 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87;
    }

    h += (unsigned long long)l;

    // Compute the hash
    while (p + 8 <= end)
    {

        // Initialized data
        unsigned long long k1 = *(const unsigned long long*)(p);
        
        k1 *= 0xC2B2AE3D27D4EB4F;
        k1 = rotl64(k1, 31);
        k1 *= 0x9E3779B185EBCA87;
        h ^= k1;
        h = rotl64(h, 27) * 0x9E3779B185EBCA87 + 0x85EBCA77C2B2AE63;
        p += 8;
    }

    if ( p + 4 <= end )
        h ^= (unsigned long long)(*(const unsigned long*)(p)) * 0x9E3779B185EBCA87,
        h = rotl64(h, 23) * 0xC2B2AE3D27D4EB4F + 0x165667B19E3779F9,
        p += 4;

    // Compute the last 8 bytes
    while ( p < end )
    {
        h ^= (*p) * 0x27D4EB2F165667C5;
        h = rotl64(h, 11) * 0x9E3779B185EBCA87, p++;
    }

    // Last round
    h ^= h >> 33, h *= 0xC2B2AE3D27D4EB4F,
    h ^= h >> 29, h *= 0x165667B19E3779F9,
    h ^= h >> 32;

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

hash64 hash_crc64 ( const void *const k, size_t l )
{
    // Argument check
    if ( k == (void *) 0 ) goto no_k;

    // Initialized data
    const unsigned char *p = k;
    hash64 h = 0xFFFFFFFFFFFFFFFF;

    // Iterate through the input
    for (size_t i = 0; i < l; i++)

        // Update the CRC
        h = crc64_table[(h ^ p[i]) & 0xFF] ^ (h >> 8);

    // Success
    return h ^ 0xFFFFFFFFFFFFFFFF;

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

int cache_construct (
    cache               **const pp_cache,
    size_t                      size,
    fn_cache_equality          *pfn_equality,
    fn_cache_key_getter        *pfn_key_get
)
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;
    if ( size     ==          0 ) goto invalid_size;

    // Initialized data
    cache *p_cache = (void *) 0;

    // Allocate memory for the cache
    if ( cache_create(&p_cache) == 0 ) goto failed_to_allocate_cache;

    // Cache starts empty
    p_cache->properties.count = 0;

    // Store the size
    p_cache->properties.max = size;

    // Set the equality function
    p_cache->pfn_equality = pfn_equality ? pfn_equality : cache_equals;

    // Set the key getter function
    p_cache->pfn_key_get = pfn_key_get ? pfn_key_get : cache_equals;

    // Allocate memory for the cache
    p_cache->properties.pp_data = HASH_CACHE_REALLOC(0, sizeof(void *) * size);

    // Error check
    if ( p_cache->properties.pp_data == (void *) 0 ) goto no_mem;

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
                
            invalid_size:
                #ifndef NDEBUG
                    log_error("[hash cache] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Hash cache errors
        {
            failed_to_allocate_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate caches in call to function \"%s\"\n", __FUNCTION__);
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
*/

int cache_get ( const cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;

    // Search the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {
        
        // Compare
        if ( p_cache->pfn_equality(p_cache->pfn_key_get(p_cache->properties.pp_data[i]), p_key ) == 0 )
        {

            // Move the property to the front of the cache 
            void *tmp = p_cache->properties.pp_data[i];
            p_cache->properties.pp_data[i] = p_cache->properties.pp_data[0];
            p_cache->properties.pp_data[0] = tmp;

            // Return the value to the caller 
            *pp_result = tmp;

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

/*
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
*/

int cache_insert ( cache *const p_cache, const void *const p_key, const void *const p_value )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;
    if ( p_value == (void *) 0 ) goto no_value;

    // If the cache is full ...
    if ( p_cache->properties.count == p_cache->properties.max )
    {
        
        // ... remove the last object in the cache ...
        //

        // ... and decrement the quantity of entries
        p_cache->properties.count--;
    }

    // Add the object to the cache
    p_cache->properties.pp_data[p_cache->properties.count] = p_value;

    // Increment the quantity of entries
    p_cache->properties.count++;

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

            no_value:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/*
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
*/

int cache_remove ( cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;
    
    /*
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
    }*/

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

int cache_clear ( cache *p_cache, fn_cache_free *pfn_cache_free )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;

    // Free each property
    if ( pfn_cache_free )
        
        // Iterate through the contents of the cache
        for (size_t i = 0; i < p_cache->properties.count; i++)
        
            // Free the property
            pfn_cache_free(p_cache->properties.pp_data[i]);

    // Clear the property counter
    p_cache->properties.count = 0;

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

int cache_for_i ( const cache *const p_cache, fn_cache_property_i pfn_function )
{
    
    // Argument check
    if ( p_cache      == (void *) 0 ) goto no_cache;
    if ( pfn_function == (void *) 0 ) goto no_function;

    // Iterate through the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)

        // Call the function
        pfn_function(p_cache->properties.pp_data[i], i);        

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

            no_function:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pfn_function\" in call to function \"%s\"\n", __FUNCTION__);
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

    // Initialized data
    cache *p_cache = *pp_cache;

    // No more pointer for caller
    *pp_cache = (void *) 0;

    // Clear the cache
    cache_clear(p_cache, pfn_cache_free);

    // Free the cache contents
    if ( HASH_CACHE_REALLOC(p_cache->properties.pp_data, 0) ) goto failed_to_free;

    // Free the cache
    if ( HASH_CACHE_REALLOC(p_cache, 0) ) goto failed_to_free;

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

        // Standard library errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to free memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/*
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

unsigned long long rotl64 ( unsigned long long x, signed char r )
{

    // Success
    return (x << r) | (x >> (64 - r));
}

int cache_equals ( const void *const p_a, const void *p_b )
{

    // Done
    return ( p_a == p_b ) ? 0 : 1;
}


