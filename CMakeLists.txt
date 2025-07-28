cmake_minimum_required(VERSION 3.20)

project(SwiftMini
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Swift Mini Language Compiler"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(SWIFT_MINI_BUILD_TESTS "Build tests" ON)
option(SWIFT_MINI_ENABLE_WARNINGS "Enable compiler warnings" ON)

if(SWIFT_MINI_ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4 /WX)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif()
endif()

add_library(SwiftMiniLib)
add_library(SwiftMini::Lib ALIAS SwiftMiniLib)

# Добавляем модули
add_subdirectory(src/lib/Parse)
add_subdirectory(src/lib/Sema)
add_subdirectory(src/lib/AST)

target_include_directories(SwiftMiniLib PUBLIC src/include)

add_executable(SwiftMini main.cpp)
target_link_libraries(SwiftMini PRIVATE SwiftMini::Lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SwiftMiniTests
    tests/test_lexer.cpp
)

target_link_libraries(SwiftMiniTests
    SwiftMiniLib
    gtest_main
)

target_include_directories(SwiftMiniTests PRIVATE
    src/include/Parse
)

include(GoogleTest)
gtest_discover_tests(SwiftMiniTests)
