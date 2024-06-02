/** !
 * Example hash cache program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>

// log
#include <log/log.h>

// hash cache
#include <hash_cache/hash_cache.h>

// Enumeration definitions
enum hash_cache_examples_e
{
    HASH_CACHE_HASH_EXAMPLE       = 0,
    HASH_CACHE_CACHE_EXAMPLE      = 1,
    HASH_CACHE_HASH_TABLE_EXAMPLE = 2,
    HASH_CACHE_EXAMPLES_QUANTITY  = 3
};

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
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

/** !
 * Hash example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_hash_example ( int argc, const char *argv[] );

/** !
 * Cache example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_cache_example ( int argc, const char *argv[] );

/** !
 * Hash table example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int hash_cache_hash_table_example ( int argc, const char *argv[] );

// Entry point
int main ( int argc, const char *argv[] )
{
    
    // Initialized data
    bool examples_to_run[HASH_CACHE_EXAMPLES_QUANTITY] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, examples_to_run);

    // Formatting
    log_info(
        "╭────────────────────╮\n"\
        "│ hash cache example │\n"\
        "╰────────────────────╯\n"\
        "The hash cache library provides hashing functions and caching abstractions.\n"\
        "Hash cache provides %d abstractions. Hashing functions, the cache, and the\n"\
        "hash table.\n\n"\
        "Hashing functions take some length of data, and produce a unique integer value.\n"\
        "A cache stores objects for quick access using a most recently used scheme.\n"
        "A hash table is used to access properties in constant time.\n\n",
        HASH_CACHE_EXAMPLES_QUANTITY
    );

    //////////////////////
    // Run the examples //
    //////////////////////

    // Run the hashing example program
    if ( examples_to_run[HASH_CACHE_HASH_EXAMPLE] )

        // Error check
        if ( hash_cache_hash_example(argc, argv) == EXIT_FAILURE ) goto failed_to_run_hash_example;

    // Run the cache example program
    if ( examples_to_run[HASH_CACHE_CACHE_EXAMPLE] )

        // Error check
        if ( hash_cache_cache_example(argc, argv) == EXIT_FAILURE ) goto failed_to_run_cache_example;

    // Run the hash table program
    if ( examples_to_run[HASH_CACHE_HASH_TABLE_EXAMPLE] )

        // Error check
        if ( hash_cache_hash_table_example(argc, argv) == EXIT_FAILURE ) goto failed_to_run_hash_table_example;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        failed_to_run_hash_example:

            // Print an error message
            log_error("Failed to run hash example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_cache_example:

            // Print an error message
            log_error("Failed to run cache example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_hash_table_example:

            // Print an error message
            log_error("Failed to run hash table example!\n");

            // Error
            return EXIT_FAILURE;
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

void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc == 1 ) goto all_examples;

    // Error check
    if ( argc > HASH_CACHE_EXAMPLES_QUANTITY + 1 ) goto invalid_arguments;

    // Iterate through each command line argument
    for (int i = 1; i < argc; i++)
    {
        
        // Hash example?
        if ( strcmp(argv[i], "hash") == 0 )
            
            // Set the thread pool example flag
            examples_to_run[HASH_CACHE_HASH_EXAMPLE] = true;

        // Cache example?
        else if ( strcmp(argv[i], "cache") == 0 )

            // Set the schedule example flag
            examples_to_run[HASH_CACHE_CACHE_EXAMPLE] = true;
        
        // Hash table example?
        else if ( strcmp(argv[i], "hash-table") == 0 )

            // Set the schedule example flag
            examples_to_run[HASH_CACHE_HASH_TABLE_EXAMPLE] = true;

        // Default
        else goto invalid_arguments;
    }
    
    // Success
    return;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < HASH_CACHE_EXAMPLES_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // Success
        return;
    }

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

int hash_cache_hash_example ( int argc, const char *argv[] )
{
    
    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭──────────────╮\n"\
        "│ hash example │\n"\
        "╰──────────────╯\n"\
        "This example hashes a string using many hashing functions. Each hash is\n"\
        "printed to standard out.\n\n"
    );

    // Initialized data
    const char _string[] = "Hi mom!";
    hash64 mmh64 = hash_mmh64(_string, sizeof(_string));
    hash64 fvn64 = hash_fnv64(_string, sizeof(_string));

    // Print each hash
    printf("mmh64(\"%s\") = 0x%016llX\n", _string, mmh64);
    printf("fvn64(\"%s\") = 0x%016llX\n", _string, fvn64);

    // Formatting
    putchar('\n');

    // Success
    return EXIT_SUCCESS;

    // Error handling
    { }
}

int hash_cache_cache_example ( int argc, const char *argv[] )
{

    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭───────────────╮\n"\
        "│ cache example │\n"\
        "╰───────────────╯\n"\
        "In this example [TODO] \n\n"\
    );

    // Initialized data
    cache *p_cache = (void *) 0;

    // Construct the cache
    if ( cache_construct(&p_cache, 64) == 0 ) goto failed_to_construct_cache;

    // Success
    return EXIT_SUCCESS;
    
    // Error handling
    {

        // Hash cache errors
        {
            failed_to_construct_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] [cache] Failed to construct cache in call to function \"%s\"\n", __FUNCTION__);
                #endif
            
                // Error
                return 0;
        }
    }
}

int hash_cache_hash_table_example ( int argc, const char *argv[] )
{
// Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭────────────────────╮\n"\
        "│ hash table example │\n"\
        "╰────────────────────╯\n"\
        "In this example [TODO] \n\n"\
    );

    // Initialized data
    //

    // Construct the hash table
    //

    // Success
    return EXIT_SUCCESS;
    
    // Error handling
    {

        // Hash cache errors
        {
            /*
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif
            
                // Error
                return 0;
            */
        }
    }
}
