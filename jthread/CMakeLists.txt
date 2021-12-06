cmake_minimum_required(VERSION 3.6)

##############
# Vcpkg integration - uncomment if necessery
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if (DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Vcpkg integration script found: " ${CMAKE_TOOLCHAIN_FILE})
else()
  message(STATUS "Vcpkg integration script NOT found!")
endif()

##################
# Project
get_filename_component(PROJECT_NAME_DIR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_ID ${PROJECT_NAME_DIR})

set(PROJECT_MAIN ${PROJECT_ID})
project(${PROJECT_MAIN})

####################
# Packages & libs
find_package(Threads)
find_package(Catch2 CONFIG REQUIRED)    

####################
# Main app
aux_source_directory(. SRC_LIST)
file(GLOB HEADERS_LIST "*.h" "*.hpp")
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${PROJECT_MAIN} PRIVATE ${PROJECT_LIB} Catch2::Catch2 Threads::Threads)
target_compile_features(${PROJECT_MAIN} PUBLIC cxx_std_20)

####################
# Tests
enable_testing() 
add_test(tests ${PROJECT_NAME})