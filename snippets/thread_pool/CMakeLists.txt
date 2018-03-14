add_executable(thread_pool
  workAB.cpp
  ThreadPool.hpp
)
target_link_libraries(thread_pool pthread)
