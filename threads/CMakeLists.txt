##############
# Vcpkg integration - uncomment if necessery
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message(STATUS "Vcpkg integration script found: " ${CMAKE_TOOLCHAIN_FILE})

get_filename_component(PROJECT_NAME_STR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME_STR})

cmake_minimum_required(VERSION 3.1)
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

#----------------------------------------
# Application
#----------------------------------------

# Sources
aux_source_directory(. SRC_LIST)

# Headers
file(GLOB HEADERS_LIST "*.h" "*.hpp")

# Application
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${PROJECT_NAME} Threads::Threads) 

# Setting C++ standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)