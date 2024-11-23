cmake_minimum_required(VERSION 3.25)

project(rsa_leakage)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

add_executable(${PROJECT_NAME} "src/main.c" "src/rsa.c" "src/server.c")
target_link_libraries(${PROJECT_NAME} PkgConfig::GMP)

