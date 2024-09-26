cmake_minimum_required(VERSION 3.25)

project(aes_leakage)

add_link_options(-static) 
set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

add_executable(${PROJECT_NAME} "src/main.c" "src/aes.c" "src/server.c")

