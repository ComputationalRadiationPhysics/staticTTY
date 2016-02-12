cmake_minimum_required(VERSION 2.6)
project(statictty)

set (CMAKE_CXX_COMPILER "clang++")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

find_package(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_executable(statictty main.cpp)
target_link_libraries(statictty ${LIBS})

install(TARGETS statictty RUNTIME DESTINATION bin)
