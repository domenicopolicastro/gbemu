cmake_minimum_required(VERSION 3.16)
project(gbemu CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

add_executable(gbemu 
    src/main.cpp
    src/Bus.cpp
    src/Cpu.cpp
    src/Timer.cpp
    src/Ppu.cpp
)

 add_executable(cpu_test
    tests/cpu_test.cpp
    src/Cpu.cpp
    src/Bus.cpp
    src/Timer.cpp
    src/Ppu.cpp
)
   
target_link_libraries(gbemu PRIVATE SDL2::SDL2)
target_compile_options(gbemu PRIVATE -Wall -Wextra)
