cmake_minimum_required(VERSION 3.12)
project(clionsql)

set(CMAKE_CXX_STANDARD 14)
find_package( Threads )

add_executable(clionsql main.cpp DBManager.cpp DBManager.h)


target_link_libraries( clionsql ${CMAKE_THREAD_LIBS_INIT} )

include_directories(/usr/include)
link_directories(/usr/lib)
target_link_libraries(clionsql sqlite3)
