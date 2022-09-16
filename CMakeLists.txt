cmake_minimum_required(VERSION 3.23)
project(ttl)

set(CMAKE_CXX_STANDARD 14)

add_executable(ttl src/ttl.cpp src/ttl.h src/utils.cpp src/utils.h src/stopwatch.cpp src/stopwatch.h src/ttl.h)

find_package(Boost COMPONENTS log log_setup filesystem program_options REQUIRED)
INCLUDE_DIRECTORIES(${boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

