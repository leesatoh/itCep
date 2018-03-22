add_executable(boost_poll_notwait
  poll_notwait.cpp
)
add_executable(boost_run_connect_timeout
  run_connect_timeout.cpp
)
target_link_libraries(boost_poll_notwait pthread boost_system)
target_link_libraries(boost_run_connect_timeout pthread boost_system)
