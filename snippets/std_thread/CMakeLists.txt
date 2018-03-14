add_executable(std_thread
  workAB.cpp
  worker.hpp
)
target_link_libraries(std_thread pthread)
