cmake_minimum_required(VERSION 3.24)
project(com_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ltbb -lpthread -pthread -O0")

add_executable(com_test
        main.cpp
        modules/Serial_impl.h
        modules/Serial_impl.cpp
        modules/TSQueue.h
        modules/FFT_class.h
        modules/FFT_class.cpp
        tests/test_com.h
        tests/test_com_plus_fft.h modules/Serial_impl.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE tbb)
