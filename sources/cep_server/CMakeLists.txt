project(CepServerProject CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(cep_server
        main.cpp
        CInputEvent.hpp
        )


target_link_libraries(cep_server boost_system pthread
        )
