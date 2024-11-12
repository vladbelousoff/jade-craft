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
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        gl3w
        GIT_REPOSITORY https://github.com/vladbelousoff/gl3w.git
        GIT_TAG 1.0.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gl3w)

set(
        EXTERNAL_LIBRARIES
        glfw
        gl3w
        StormLib::storm
        Glob
)

add_subdirectory(jade)

file(GLOB_RECURSE SOURCE_FILES "wow/*.cpp")
file(GLOB_RECURSE HEADER_FILES "wow/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} JadeLib ${EXTERNAL_LIBRARIES})

