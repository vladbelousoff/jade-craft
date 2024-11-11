cmake_minimum_required(VERSION 3.29)
project(JadeCraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        StormLib
        GIT_REPOSITORY https://github.com/ladislav-zezula/StormLib.git
        GIT_TAG v9.26
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(StormLib)

# Assuming StormLib does not provide a target, manually specify the include directory
add_library(StormLib INTERFACE)
target_include_directories(StormLib INTERFACE "${StormLib_SOURCE_DIR}/src")

set(
        EXTERNAL_LIBRARIES
        StormLib::storm
)

add_subdirectory(src/jade)

add_executable(${PROJECT_NAME} JadeCraft.cpp)
target_link_libraries(${PROJECT_NAME} JadeLib ${EXTERNAL_LIBRARIES})

