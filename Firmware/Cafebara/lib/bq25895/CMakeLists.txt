cmake_minimum_required(VERSION 3.10)
project(bq24292i VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(bq24292i STATIC src/bq24292i.c)

install(TARGETS bq24292i DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

target_include_directories(
  bq24292i PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
