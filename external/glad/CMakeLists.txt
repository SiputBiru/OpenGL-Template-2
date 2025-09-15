cmake_minimum_required(VERSION 4.1)

add_library(glad STATIC src/glad.c)
# target_sources(glad PRIVATE src/glad.c)
# target_include_directories(glad
#   PUBLIC include
# )
#
target_include_directories(glad PUBLIC include)

