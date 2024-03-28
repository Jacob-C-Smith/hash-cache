# HASH CACHE CMake configuration file:
# This file is meant to be placed in a cmake subfolder of HASH_CACHE-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(HASH_CACHE PROPERTIES
    URL "https://g10.app/status/#primitives"
    DESCRIPTION "Abstractions for hashing and caching data"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(HASH_CACHE_FOUND TRUE)

# For compatibility with autotools HASH_CACHE-config.cmake, provide HASH_CACHE_* variables.

set_and_check(HASH_CACHE_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(HASH_CACHE_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(HASH_CACHE_INCLUDE_DIR  "${HASH_CACHE_PREFIX}/include")
set(HASH_CACHE_INCLUDE_DIRS           "${HASH_CACHE_INCLUDE_DIR}")
set_and_check(HASH_CACHE_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(HASH_CACHE_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(HASH_CACHE_LIBRARIES hash_cache::hash_cache)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated HASH_CACHE-target.cmake files.

set(_HASH_CACHE_library     "${HASH_CACHE_LIBDIR}/hash_cache.lib")
set(_HASH_CACHE_dll_library "${HASH_CACHE_BINDIR}/hash_cache.dll")
if(EXISTS "${_HASH_CACHE_library}" AND EXISTS "${_HASH_CACHE_dll_library}")
    if(NOT TARGET hash_cache::hash_cache)
        add_library(hash_cache::hash_cache SHARED IMPORTED)
        set_target_properties(hash_cache::hash_cache
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${HASH_CACHE_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_HASH_CACHE_library}"
                IMPORTED_LOCATION "${_HASH_CACHE_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "HASH_CACHE_SHARED"
                INTERFACE_HASH_CACHE_SHARED "ON"
        )
    endif()
    set(HASH_CACHE_HASH_CACHE_FOUND TRUE)
else()
    set(HASH_CACHE_HASH_CACHE_FOUND FALSE)
endif()
unset(_HASH_CACHE_library)
unset(_HASH_CACHE_dll_library)

check_required_components(HASH_CACHE)
