cmake_minimum_required(VERSION 3.10)
project(benchmark_llama VERSION 1.0)

# set cxx standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(WARNING "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS_RELEASE  " -O3 -std=c++17  -march=native -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -std=c++17 -march=native")

add_definitions(-DTEST -DNITER=10)

if (NOT DEFINED NTHREADS)
    set(NTHREADS 1)
endif()

# pass variable to source code
message(WARNING "NTHREADS: ${NTHREADS}")
add_definitions(-DNTHREADS=${NTHREADS})

# Sets variable to a value if variable is unset.
macro(set_ifndef var val)
    if (NOT ${var})
        set(${var} ${val})
    endif()
    message(STATUS "Configurable variable ${var} set to ${${var}}")
endmacro()

set_ifndef(TOP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)


# === Compile kernels ===
add_subdirectory("A8-W4-GGML")
add_subdirectory("A80-W40-simd_aware_weight_packing")
add_subdirectory("A81-W41-simd_aware_weight_packing")
add_subdirectory("A80-W40-simd_aware_weight_packing-repack")
add_subdirectory("A81-W41-simd_aware_weight_packing-repack")
add_subdirectory("Afp32-Wfp32-BLIS_implementation")

