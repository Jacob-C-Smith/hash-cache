/** !
 * hash table library
 *
 * @file hash_table.c
 * @date Feb 18, 2024
 * @author Jacob Smith
 */

// Headers
#include <hash_cache/cache.h>

struct hash_table_s
{
    struct
    {
        size_t   max;
        void   **pp_data;
    } entries;

     pfn_hash_function;
    size_t key_size;
    // TODO: Equality function
};

typedef struct hash_table_s hash_table;

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

signed hash_table_positive_mod ( signed dividend, signed divisor )
{

    // Initialized data
    signed quotient = dividend % divisor;

    // Success
    return ( quotient < 0 ) ? quotient + divisor : quotient;
}

unsigned hash_table_double_hash ( hash_table *p_hash_table, void *p_key, signed i )
{

    // Initialized data
    unsigned k = p_hash_table->pfn_hash_function(p_key, p_hash_table->key_size);
    
    // Success
    return ( ( hash_table_positive_mod(p_key, p_hash_table->entries.max) ) + i * ( 1 + hash_table_positive_mod( k, p_hash_table->entries.max - 2 ) ) );
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