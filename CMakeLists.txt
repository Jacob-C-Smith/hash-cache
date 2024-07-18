# Need CMake 3.16
cmake_minimum_required (VERSION 3.16.0)

# The name of the repository
project ("hash cache")

# Set compiler warnings
if(MSVC)
    # TODO
else()

    # For clang
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -std=gnu99 -Wall -Wextra -Weverything -Wno-declaration-after-statement -Ofast")

    # For GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra -O3 -pthread -lpthread")
    add_compile_definitions(__USE_POSIX199309)
    add_compile_options(-Wall -Wextra -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -pthread -lpthread -Wlogical-not-parentheses -Wnull-dereference)
endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set the output paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Build sync with semaphore
add_compile_definitions(BUILD_SYNC_WITH_SEMAPHORE)

# Build sync with monitor
add_compile_definitions(BUILD_SYNC_WITH_MONITOR)

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")
    
    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()


# Add the hash_cache project
if ( NOT "${HAS_HASH_CACHE}")

    set (HASH_CACHE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the log project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_HASH_CACHE true PARENT_SCOPE)
        set(HASH_CACHE_INCLUDE_DIR ${HASH_CACHE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (hash_cache_example "main.c")
add_dependencies(hash_cache_example hash_cache log sync)
target_include_directories(hash_cache_example PUBLIC ${HASH_CACHE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(hash_cache_example hash_cache log sync)

# Add source to this project's library
add_library (hash_cache SHARED "hash_cache.c" "hash.c" "cache.c" "hash_table.c")
add_dependencies(hash_cache log sync)
target_include_directories(hash_cache PUBLIC ${HASH_CACHE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(hash_cache PRIVATE log sync)
