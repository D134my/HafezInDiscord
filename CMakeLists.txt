cmake_minimum_required(VERSION 3.14)
project(hafez LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/DPP)

add_executable(hafez
  main.cpp hafez.cpp hafez.hpp
)

target_link_libraries(hafez mysqlclient  dpp)
target_include_directories(${PROJECT_NAME} PRIVATE
    libs/DPP/include
)
 
