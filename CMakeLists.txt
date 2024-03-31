cmake_minimum_required(VERSION 3.12)

include(cmake/pico_sdk_import.cmake)

project(line-follower)

pico_sdk_init()

add_executable(line-follower src/main.c)

target_link_libraries(line-follower pico_stdlib)

pico_add_extra_outputs(line-follower)
