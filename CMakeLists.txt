cmake_minimum_required(VERSION 3.20)
project(byteknife VERSION 0.1.3 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# unity builds
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)

# ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    message(STATUS "Using ccache")
endif()

# mold linker (much faster than default ld/lld for large link steps)
find_program(MOLD_PROGRAM mold)
if(MOLD_PROGRAM)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
    message(STATUS "Using mold linker")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)

# Zydis
add_subdirectory(.libraries/zydis)


# byteknife sources
file(GLOB_RECURSE CPP_SOURCES
    CONFIGURE_DEPENDS
    source/*.cpp
)
file(GLOB_RECURSE C_SOURCES
    CONFIGURE_DEPENDS
    source/*.c
)

add_executable(${PROJECT_NAME}
    ${CPP_SOURCES}
    ${C_SOURCES}
)

# precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/source/pch.h
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-O0 -g>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        Qt6::Concurrent
        Zydis
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

# disable unity for zydis
set_target_properties(Zydis PROPERTIES UNITY_BUILD OFF)
set_target_properties(Zycore PROPERTIES UNITY_BUILD OFF)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/dist
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/.assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
