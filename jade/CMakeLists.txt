cmake_minimum_required(VERSION 3.29)
project(jade-lib)

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
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.9
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
        gl3w
        GIT_REPOSITORY https://github.com/vladbelousoff/gl3w.git
        GIT_TAG 1.0.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gl3w)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)

set(
        EXTERNAL_LIBRARIES
        SDL2::SDL2-static
        SDL2::SDL2main
        gl3w
        spdlog
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTERNAL_LIBRARIES})

if (JADE_ENABLE_LIBASSERT)
    FetchContent_Declare(
            libassert
            GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
            GIT_TAG v2.1.2
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(libassert)

    target_link_libraries(${PROJECT_NAME} PUBLIC libassert::assert)
    target_compile_definitions(${PROJECT_NAME} PUBLIC JADE_ENABLE_LIBASSERT)
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

    # Find all test source files
    file(GLOB TEST_SOURCES "test/*.cpp")

    # Loop through each test source file and create an executable
    foreach (TEST_FILE ${TEST_SOURCES})
        # Get the filename without directory or extension
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        # Create an executable for the test file
        add_executable(${PROJECT_NAME}-test_${TEST_NAME} ${TEST_FILE})

        # Link the executable to the project library and Catch2
        target_link_libraries(${PROJECT_NAME}-test_${TEST_NAME}
                ${PROJECT_NAME}
                Catch2::Catch2WithMain
        )
    endforeach ()
endif ()

find_library(D3D9_LIBRARY d3d9)

if (D3D9_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${D3D9_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PUBLIC JADE_D3D9_SUPPORT)
endif ()

find_library(D3D11_LIBRARY d3d11)

if (D3D11_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${D3D11_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PUBLIC JADE_D3D11_SUPPORT)
endif ()
