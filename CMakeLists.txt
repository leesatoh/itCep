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
add_subdirectory(snippets/boost_poll)
add_subdirectory(snippets/boost_io_service)
add_subdirectory(snippets/boost_wakayama)
add_subdirectory(snippets/boost_async_write_problem)
add_subdirectory(snippets/signal_connect_model)
add_subdirectory(snippets/boost_workpool)
