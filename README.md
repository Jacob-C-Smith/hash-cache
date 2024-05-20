# hash cache
[![CMake](https://github.com/Jacob-C-Smith/hash-cache/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/hash-cache/actions/workflows/cmake.yml)

**Dependencies:**\
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml) [![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 Abstractions for hashing and caching data.

 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example output](#example-output)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)

 ## Download
 To download hash cache, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/hash-cache
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd hash-cache
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build hash cache for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
TODO
 ### Example output
 TODO
 [Source](main.c)
## TODO: Tester


 ## Definitions
 ### Type definitions
 ```c
// Type definitions
typedef unsigned long long hash64;
typedef hash64 (fn_hash64)( const void *const k, size_t l );
 ```
 ### Function definitions
 ```c
 // Hashing
 hash64 hash_mmh64 ( const void* const k, size_t l );
 hash64 hash_fnv64 ( const void* const k, size_t l );
 ```

