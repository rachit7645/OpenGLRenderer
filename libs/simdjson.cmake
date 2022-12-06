# SIMD JSON source files
add_library(simdjson STATIC simdjson/singleheader/simdjson.cpp)

# SIMD JSON precompiled headers
target_precompile_headers(simdjson PRIVATE simdjson/singleheader/simdjson.h)

# SIMD JSON includes
target_include_directories(simdjson PUBLIC simdjson/singleheader/)
# Disable warnings
target_compile_options(simdjson PRIVATE -w)
# -O3 optimisations
target_compile_options(simdjson PRIVATE -O3)
# Debug output
target_compile_options(simdjson PRIVATE -g)
# Enable SSE2
target_compile_options(simdjson PRIVATE -msse2)
# SIMD JSON libs
target_link_libraries(simdjson)