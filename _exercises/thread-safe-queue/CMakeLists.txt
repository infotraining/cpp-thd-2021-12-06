get_filename_component(PROJECT_NAME_STR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME_STR})

cmake_minimum_required(VERSION 3.6)
project(${PROJECT_NAME_STR})

#----------------------------------------
# set compiler
#----------------------------------------
if (MSVC)
    add_compile_options(-D_SCL_SECURE_NO_WARNINGS)
endif()

#----------------------------------------
# set Threads
#----------------------------------------
find_package(Threads REQUIRED)

add_subdirectory(src)

#----------------------------------------
# Application
#----------------------------------------
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads thread_safe_queue_lib)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)


#----------------------------------------
# Tests
#----------------------------------------
enable_testing(true)
add_subdirectory(tests)
add_test(unit_tests tests/thread_safe_queue_tests)