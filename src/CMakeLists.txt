project(token)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER clang)

file(GLOB intp "./" "*.cxx")

add_executable(interpreter
    ${intp}
)

target_compile_options(interpreter PUBLIC
    -g
    -O1
    -fsanitize=address
    -fno-omit-frame-pointer
    -Wall
    -stdlib=libstdc++
)


target_link_libraries(interpreter
    pthread
    dl
    rt
    asan.a
    fmt.a
)