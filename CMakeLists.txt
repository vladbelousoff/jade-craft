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

FetchContent_Declare(
        GlobLib
        GIT_REPOSITORY https://github.com/p-ranav/glob.git
        GIT_TAG v0.0.1
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(GlobLib)

FetchContent_Declare(
        LuaBridge
        GIT_REPOSITORY https://github.com/vinniefalco/LuaBridge.git
        GIT_TAG 2.8
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(LuaBridge)

set(
        WOW_EXTERNAL_LIBRARIES
        StormLib::storm
        Glob
        LuaBridge
)

add_subdirectory(jade)

file(GLOB_RECURSE SOURCE_FILES "wow/*.cpp")
file(GLOB_RECURSE HEADER_FILES "wow/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE JadeLib ${WOW_EXTERNAL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "wow")

