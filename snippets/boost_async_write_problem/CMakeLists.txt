add_executable(problem problem.cpp)
target_link_libraries(problem boost_system pthread)


add_executable(result result.cpp)
target_link_libraries(result boost_system pthread)
