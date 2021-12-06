##############
# Vcpkg integration - uncomment if necessery
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message(STATUS "Vcpkg integration script found: " ${CMAKE_TOOLCHAIN_FILE})

get_filename_component(PROJECT_NAME_STR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME_STR})

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME_STR})
set(CMAKE_BUILD_TYPE "Release") 

#----------------------------------------
# Libraries
#----------------------------------------
find_package(Threads REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

#----------------------------------------
# Application
#----------------------------------------

# Sources
aux_source_directory(. SRC_LIST)

# Headers
file(GLOB HEADERS_LIST "*.h" "*.hpp")
include_directories(${Boost_INCLUDE_DIRS} ${CELERO_INCLUDE_DIRS})

# Application
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads Catch2::Catch2)

# Setting C++ standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (MSVC)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -D_SCL_SECURE_NO_WARNINGS)
else()
  find_package(TBB CONFIG REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)
endif()
