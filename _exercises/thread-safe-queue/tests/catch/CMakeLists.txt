project (Catch)

# Header only library, therefore INTERFACE
add_library(catch_lib INTERFACE)

# INTERFACE targets only have INTERFACE properties
target_include_directories(catch_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)