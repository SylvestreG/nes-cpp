cmake_minimum_required(VERSION 3.17)
project(nes_cpp)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(nes_cpp
        src/cartridge/cartridge.cpp
        src/cpu/cpu_mem_bus.cpp
        src/cpu/decoder.cpp
        src/cpu/execute.cpp
        src/memory/block.cpp
        src/main.cpp include/cpu/execute.h src/cpu/execute.cpp src/hex_editor.h)
target_link_libraries(nes_cpp CONAN_PKG::sfml CONAN_PKG::imgui-sfml CONAN_PKG::spdlog)