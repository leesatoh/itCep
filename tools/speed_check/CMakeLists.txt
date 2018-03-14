add_executable(CepChecker
 CepChecker.cpp
 CepMessage.hpp
 InterFaces.h
 ThreadPool.hpp
 WorkSndRcv.hpp
)
target_link_libraries(CepChecker boost_system pthread)
