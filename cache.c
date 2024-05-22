/** !
 * Implementation of cache
 *
 * @file cache.c
 *
 * @author Jacob Smith
 */

// Headers
#include <hash_cache/cache.h>

struct cache_s
{

    size_t    key_size,
              keys,
              max;
    fn_hash64  *pfn_hash;
    int       (*pfn_equality)(const void *const p_a, const void *const p_b);
    void       *_p_data[];
};

/** !
 * Is an integer a prime number?
 * 
 * @param n the integer 
 * 
 * @return true if prime else false
 */
bool hash_table_is_prime ( int n )
{

    // Iterate from 2 to the upper bound
    for (size_t i = 2; i < n; i++)

        // Check for a remainder
        if ( n % i == 0 )

            // Not a prime
            return false;
    
    // A prime
    return true;
}

/** !
 * Generate a twin prime within a range
 * 
 * @param start the lower bound of the range
 * @param end   the upper bound of the range
 * 
 * @return the prime number
 */
size_t hash_table_generate_twin_prime ( size_t start, size_t end )
{

    // Initialized data
    size_t j = 0;

    // Iterate through the range
    for (size_t i = start; i < end; i++)
    
        
        // Check the twin prime
        if ( hash_table_is_prime(i) && hash_table_is_prime(i+2) )

            // Success
            return i + 2;
    
    // Error
    return 0;
}

/** !
 * Compute the positive modulo of some numbers
 * 
 * @param dividend the dividend
 * @param divisor  the divisor
 * 
 * @return the positive modulo of the dividend and the divisor
 */
signed hash_table_positive_mod ( signed dividend, signed divisor )
{

    // Initialized data
    signed quotient = dividend % divisor;

    // Success
    return ( quotient < 0 ) ? quotient + divisor : quotient;
}

unsigned hash_table_double_hash ( cache *p_cache, void *p_key, signed i )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;

    // Initialized data
    unsigned k = p_cache->pfn_hash(p_key, p_cache->key_size);
    
    // Success
    return ( ( hash_table_positive_mod(p_key, p_cache->max) ) + i * ( 1 + hash_table_positive_mod( k, p_cache->max - 2 ) ) );

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    printf("[hash cache] [cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/*
int hash_table_create ( hash_table **pp_hash_table )
{



    // Success
    return 1;
}

int hash_table_construct ( hash_table **pp_hash_table, crypto_hash_function_64_t *pfn_hash_function )
{

    // Success
    return 1;
}

signed hash_table_insert ( hash_table *p_hash_table, void *p_key )
{

    // Initialized data
    signed i = 0;
    signed p = 0;

    // Repeat this block
    do
    {
        
        // Initialized data
        signed q = hash_table_double_hash(p_hash_table, p_key, i);

        // Update the probe counter
        p++;

        // If the slot is empty ...
        if ( p_hash_table->entries.pp_data[q] == (void *) 0 )
        {

            // ... store the key ...
            p_hash_table->entries.pp_data[q] = p_key;

            // ... and return the index
            return q;
        } 

        // Increment the counter
        i++;
    }
    
    // Continuation condition
    while ( i != p_hash_table->entries.max);

    // Error
    return -1;
}

signed hash_table_search ( hash_table *p_hash_table, void *p_key )
{

    // Initialized data
    signed i = 0;
    signed q = 0;

    // Repeat this block
    do
    {
        
        // Compute the hash
        q = hash_table_double_hash(p_hash_table, p_key, i);

        // If the slot is occupied ...
        if ( p_hash_table->entries.pp_data[q] != (void *) 0 )

            // ...and the slot contains the key ...
            if ( p_hash_table->entries.pp_data[q] == p_key )

                // ... return the index
                return q; 

        // Increment the counter
        i++;
    }
    
    // Continuation condition
    while ( p_hash_table->entries.pp_data[q] != (void *) 0 && i != p_hash_table->entries.max);

    // Error
    return -1;
}

int hash_table_destruct ( hash_table **pp_hash_table )
{

    // Success
    return 1;
}
*/
