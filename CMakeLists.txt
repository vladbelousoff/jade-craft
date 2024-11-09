cmake_minimum_required(VERSION 3.29)

project(JadeCraft)

include(FetchContent)

option(JADE_ENABLE_TESTS "Enable Tests" ON)
option(JADE_ENABLE_LIBASSERT "Enable Libassert" ON)

if (JADE_ENABLE_LIBASSERT)
    FetchContent_Declare(
            libassert
            GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
            GIT_TAG v2.1.2
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(libassert)
endif ()

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        StormLib
        GIT_REPOSITORY https://github.com/ladislav-zezula/StormLib.git
        GIT_TAG v9.26
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(StormLib)

# Assuming StormLib does not provide a target, manually specify the include directory
add_library(StormLib INTERFACE)
target_include_directories(StormLib INTERFACE ${StormLib_SOURCE_DIR}/src)

set(
        EXTERNAL_LIBRARIES
        glfw
        StormLib::storm
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE JADE_SOURCE_FILES "src/jade/*.cpp")
file(GLOB_RECURSE JADE_HEADER_FILES "src/jade/*.hpp")

add_library(JadeCraftLib STATIC ${JADE_SOURCE_FILES} ${JADE_HEADER_FILES})
target_include_directories(JadeCraftLib PUBLIC "src")

if (JADE_ENABLE_LIBASSERT)
    target_link_libraries(JadeCraftLib libassert::assert)
    target_compile_definitions(JadeCraftLib PRIVATE JADE_ENABLE_LIBASSERT=1)
endif ()

add_executable(${PROJECT_NAME} JadeCraft.cpp)
target_link_libraries(${PROJECT_NAME} JadeCraftLib ${EXTERNAL_LIBRARIES})

if (JADE_ENABLE_TESTS)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(${PROJECT_NAME}Test JadeCraftTest.cpp)
    target_link_libraries(${PROJECT_NAME}Test
            JadeCraftLib
            ${EXTERNAL_LIBRARIES}
            Catch2::Catch2WithMain
    )
endif ()

