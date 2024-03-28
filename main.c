// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <hash_cache/hash.h>

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialized data
    const char _string[] = "Hi mom!";
    hash64 mmh = hash_mmh64(_string, sizeof(_string));
    hash64 fvn = hash_fnv64(_string, sizeof(_string));

    // Print each hash
    printf("mmh(\"%s\") = 0x%016llx\n", _string, mmh);
    printf("fvn(\"%s\") = 0x%016llx\n", _string, fvn);

    // Success
    return EXIT_SUCCESS;
}
