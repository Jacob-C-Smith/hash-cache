/** !
 * A tool for creating optimally small 
 * hash tables without collisions
 * 
 * @file hash_table_optimizer.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>

// log
#include <log/log.h>

// hash cache
#include <hash_cache/hash.h>

// Preprocessor definitions
#define HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX 4095+1

// Enumeration definitions
enum hash_table_optimizer_hash_functions_e
{
    HASH_TABLE_OPTIMIZER_CRC64    = 0,
    HASH_TABLE_OPTIMIZER_FNV64    = 1,
    HASH_TABLE_OPTIMIZER_MMH64    = 2,
    HASH_TABLE_OPTIMIZER_QUANTITY = 3
};

// Structure definitions
struct quasi_hash_table_entry_s
{
    bool  occupied;
    char *value;
};

struct quasi_hash_table_s
{
    size_t len;
    struct quasi_hash_table_entry_s data[];
};

struct quasi_hash_table_property_s 
{
    size_t  len;
    char    _text[];
};

// Type definitions
typedef struct quasi_hash_table_s          quasi_hash_table;
typedef struct quasi_hash_table_property_s quasi_hash_table_property;


// Forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc               the argc parameter of the entry point
 * @param argv               the argv parameter of the entry point
 * @param ppfn_hash_function result
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], fn_hash64 **ppfn_hash_function );

// Entry point
int main ( int argc, const char *argv[] )
{
    
    // Initialized data
    size_t entry_quantity = 0, entry_max = 1;
    size_t hash_table_test_size = 0;
    char _buffer[HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX] = { 0 };
    quasi_hash_table_property **pp_properties = realloc(0, sizeof(void *) * entry_max);
    fn_hash64 *pfn_hashing_function = (void *) 0;
    quasi_hash_table *p_quasi_hash_table = (void *) 0;
    
    // Error check
    if ( pp_properties == (void *) 0 ) goto failed_to_realloc;

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, &pfn_hashing_function);

    // Read until EOF
    while ( !feof(stdin) )
    {
        
        // Initialized data
        size_t                     len      = 0;
        quasi_hash_table_property *p_string = (void *) 0;

        // Read a string from standard in
        if ( fgets(&_buffer, HASH_TABLE_OPTIMIZER_BUFFER_LENGTH_MAX, stdin) == 0 ) break;

        // Resize?
        if ( entry_max == entry_quantity ) 
        {

            // Double the maximum
            entry_max *= 2;

            // Grow the allocation
            pp_properties = realloc(pp_properties, sizeof(quasi_hash_table_property *) * entry_max);

            // Error check 
            if ( pp_properties == (void *) 0 ) goto failed_to_realloc; 
        }

        // Compute the length of the entry
        len = strlen(_buffer);

        // Allocate memory for the entry
        p_string = realloc(0, sizeof(quasi_hash_table_property) + len * sizeof(char));

        // Error check
        if ( p_string == (void *) 0 ) goto failed_to_realloc;

        // Copy the string from the stack to the heap
        strncpy(&p_string->_text, _buffer, len-1);

        // Store a null terminator
        p_string->_text[len - 1] = '\0';

        // Store the length of the string on the heap
        p_string->len = len;

        // Add the entry to the buffer
        pp_properties[entry_quantity] = p_string;

        // Increment the quantity of elements
        entry_quantity++;
    }
    
    // Store the entry quantity
    hash_table_test_size = entry_quantity;
    
    p_quasi_hash_table = HASH_CACHE_REALLOC(0, sizeof(struct quasi_hash_table_s));

    while (1)
    {

        p_quasi_hash_table = HASH_CACHE_REALLOC(p_quasi_hash_table, sizeof(struct quasi_hash_table_s) + sizeof(struct quasi_hash_table_entry_s) * hash_table_test_size);

        memset(p_quasi_hash_table, 0, sizeof(struct quasi_hash_table_s) + sizeof(struct quasi_hash_table_entry_s) * hash_table_test_size);

        p_quasi_hash_table->len = hash_table_test_size;

        for (size_t i = 0; i < entry_quantity; i++)
        {
            
            // Initialized data
            hash64 hash  = pfn_hashing_function(pp_properties[i]->_text, pp_properties[i]->len);
            size_t index = hash % hash_table_test_size;

            // Test if the bit is occupied
            if ( p_quasi_hash_table->data[index].occupied )
                
                // Try again ...
                goto try_again;
            
            // Set the occupied flag
            p_quasi_hash_table->data[index].occupied = true;
            p_quasi_hash_table->data[index].value    = &pp_properties[i]->_text;
            
        }

        // We done
        break;

        try_again:
        
            // ... with a larger table
            hash_table_test_size++;

            continue;
    }
    
    printf("const char *_p_hash_table[%d] = {", p_quasi_hash_table->len);

    for (size_t i = 0; i < p_quasi_hash_table->len; i++)
    {
        if ( p_quasi_hash_table->data[i].occupied )
            printf("\"%s\"%c", p_quasi_hash_table->data[i].value, (p_quasi_hash_table->len == (i - 1)) ? ' ' : ',');
        else
            printf("(void *) 0%c", (p_quasi_hash_table->len == (i - 1)) ? ' ' : ',');
    }

    printf("};\n");

    // Clean up
    for (size_t i = 0; i < entry_quantity; i++)

        // Release 
        pp_properties[i] = realloc(pp_properties[i], 0);
    
    pp_properties = realloc(pp_properties, 0);

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

        // Standard library errors
        {
            failed_to_realloc:
                return EXIT_FAILURE;
        }
    }
}

void print_usage ( const char *argv0 )
{

    // Argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [hash] [cache] [hash-table]\n", argv0);

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], fn_hash64 **ppfn_hash_function )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc != 2 ) goto invalid_arguments;

    // CRC
    if ( strcmp(argv[1], "crc") == 0 )
        
        // Set the thread pool example flag
        *ppfn_hash_function = hash_crc64;

    // Cache example?
    else if ( strcmp(argv[1], "fnv") == 0 )

        // Fowler-Noll-Vo
        *ppfn_hash_function = hash_fnv64;
    
    // Hash table example?
    else if ( strcmp(argv[1], "mmh") == 0 )

        // MurMur
        *ppfn_hash_function = hash_mmh64;

    // Default
    else goto invalid_arguments;
    
    // Success
    return;
    
    // Error handling
    {

        // Argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}
