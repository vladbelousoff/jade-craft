cmake_minimum_required(VERSION 3.29)
project(JadeLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(JADE_ENABLE_TESTS "Enable Catch2" ON)
option(JADE_ENABLE_LIBASSERT "Enable Libassert" ON)

file(GLOB_RECURSE JADE_SOURCE_FILES "jade/*.cpp")
file(GLOB_RECURSE JADE_HEADER_FILES "jade/*.hpp")

add_library(${PROJECT_NAME} STATIC ${JADE_SOURCE_FILES} ${JADE_HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog)

if (JADE_ENABLE_LIBASSERT)
    FetchContent_Declare(
            libassert
            GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
            GIT_TAG v2.1.2
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(libassert)

    target_link_libraries(${PROJECT_NAME} PUBLIC libassert::assert)
    target_compile_definitions(${PROJECT_NAME} PUBLIC JADE_ENABLE_LIBASSERT=1)
endif ()

if (JADE_ENABLE_TESTS)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    target_include_directories(Catch2 INTERFACE "${Catch2_SOURCE_DIR}/src")

    add_executable(${PROJECT_NAME}Test test/test.cpp)
    target_link_libraries(${PROJECT_NAME}Test
            ${PROJECT_NAME}
            Catch2::Catch2WithMain
    )
endif ()
