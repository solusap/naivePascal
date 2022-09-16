project(token)
cmake_minimum_required(VERSION 3.16)


set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER clang)
file(GLOB src_list "./" "token.cxx")

file(GLOB intp "./" "interpreter.cxx")

add_library(token
    ${src_list}
)

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
    token
    fmt.a
)