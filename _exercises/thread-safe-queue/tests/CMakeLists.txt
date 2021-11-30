project (thread_safe_queue_tests)

add_subdirectory(catch)

find_package(Threads REQUIRED)

add_executable(thread_safe_queue_tests thread_safe_queue_tests.cpp main_tests.cpp)
target_link_libraries(thread_safe_queue_tests PRIVATE thread_safe_queue_lib catch_lib Threads::Threads)