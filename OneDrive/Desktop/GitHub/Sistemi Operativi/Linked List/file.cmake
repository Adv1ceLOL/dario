cmake_minimum_required(VERSION 2.8.11)
project(porco_cane)

add_library(linked_list_library SHARED
    linked_list.c
    int_list.c
    float_list.c
)

add_executable(linked_list_test
    linked_list_test.c)

target_link_libraries(linked_list_test
    linked_list_library)

add_executable(float_linked_list
    float_linked_list.c)

target_link_libraries(float_linked_list
    linked_list_library)

include_directories(${PROJECT_SOURCE_DIR})
