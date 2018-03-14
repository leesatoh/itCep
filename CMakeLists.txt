cmake_minimum_required(VERSION 2.8)
project(itCepProject)
add_definitions(-g -Wall -Wno-unused-result -libstdc++)
set(CMAKE_EXE_LINKER_FLAGS "-lstdc++")

#main source
add_subdirectory(sources/cep_server)

#main source
add_subdirectory(tools/speed_check)

#developing snippets(no longer need) 
add_subdirectory(snippets/small)
add_subdirectory(snippets/std_thread)
add_subdirectory(snippets/boost_io)
add_subdirectory(snippets/check_time)
